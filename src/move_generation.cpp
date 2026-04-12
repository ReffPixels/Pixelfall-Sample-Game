// [TODO] Clean up and describe
#include "move_generation.h"

void moveGeneration::findLegalMovesForPiece(std::array<std::array<MoveType, 8>, 8>& moves,
    Tile piece, Vector2Int position, const std::array<std::array<Tile, 8>, 8>& boardState,
    const CastlingRights& castlingRights, Vector2Int enPassantTargetSquare) {

    // Iterate through every pseudo legal move
    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            if (moves[f][r] == MoveType::None)
                continue; // Not a valid move, keep looking

            // Create a board that represents the next turn if this move was made 
            std::array<std::array<Tile, 8>, 8> simulatedBoard = boardState;
            simulatedBoard[position.x][position.y] = {PieceType::None, TeamColor::None};
            simulatedBoard[f][r] = piece;

            // En passant is an exception because the captured pawn is in a different tile than the moved pawn 
            // This is the only move where this happens. We need to clear the pawn origin AND the captured pawn.
            if (moves[f][r] == MoveType::EnPassant)
                simulatedBoard[f][position.y] = {PieceType::None, TeamColor::None};

            // Find attacked squares in simulated board
            std::array<std::array<bool, 8>, 8> unsafeSquares =
                getAttackedSquares(true, piece.team, simulatedBoard, castlingRights);

            // Find king position
            Vector2Int kingPosition = {};
            for (int kr = 0; kr < 8; kr++)
                for (int kf = 0; kf < 8; kf++)
                    if (simulatedBoard[kf][kr].team == piece.team
                        && simulatedBoard[kf][kr].type == PieceType::King)
                        kingPosition = {kf, kr};

            // Castling is not allowed if the king is under attack or any of the passing squares are under attack.
            // We don't care if the rook is under attack.
            if (moves[f][r] == MoveType::CastlingKingSide)
                if ((piece.team == TeamColor::White && (
                    unsafeSquares[4][7] || unsafeSquares[5][7] || unsafeSquares[6][7])
                        || piece.team == TeamColor::Black && (
                        unsafeSquares[4][0] || unsafeSquares[5][0] || unsafeSquares[6][0])))
                    // Remove illegal move
                    moves[f][r] = MoveType::None;
            if (moves[f][r] == MoveType::CastlingQueenSide)
                if ((piece.team == TeamColor::White && (
                    unsafeSquares[2][7] || unsafeSquares[3][7] || unsafeSquares[4][7])
                    || piece.team == TeamColor::Black && (
                        unsafeSquares[2][0] || unsafeSquares[3][0] || unsafeSquares[4][0])))
                    // Remove illegal move
                    moves[f][r] = MoveType::None;

            // The king could be captured! This move is illegal
            if (kingPosition != Vector2Int{-1, -1} && unsafeSquares[kingPosition.x][kingPosition.y]) {
                // Remove illegal move
                moves[f][r] = MoveType::None;
            }
        }
    }
}

// Dispatch move generation to the correct function based on piece type
std::array<std::array<MoveType, 8>, 8> moveGeneration::generateMovesForPiece(
    Tile piece, Vector2Int position, const std::array<std::array<Tile, 8>, 8>& boardState,
    const CastlingRights& castlingRights, Vector2Int enPassantTargetSquare) {

    switch (piece.type) {
    case PieceType::King:
        return generateKingMoves(position, piece.team, boardState, castlingRights);
    case PieceType::Queen:
        return generateQueenMoves(position, piece.team, boardState);
    case PieceType::Rook:
        return generateRookMoves(position, piece.team, boardState);
    case PieceType::Bishop:
        return generateBishopMoves(position, piece.team, boardState);
    case PieceType::Knight:
        return generateKnightMoves(position, piece.team, boardState);
    case PieceType::Pawn: {
        bool isFirstMove = (piece.team == TeamColor::White) ? (position.y == 6) : (position.y == 1);
        return generatePawnMoves(position, piece.team, boardState, enPassantTargetSquare, isFirstMove);
    }
    default:
        std::array<std::array<MoveType, 8>, 8> empty{};
        return empty;
    }
}

std::array<std::array<MoveType, 8>, 8> moveGeneration::generateKingMoves(
    Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState,
    const CastlingRights& castlingRights) {

    // Normal Moves
    std::array<std::array<MoveType, 8>, 8> kingMoves =
        generateMoves(moveOrigin, TeamColor, boardState, {
            Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left,
            Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 1);

    // White Castling Moves
    if (TeamColor == TeamColor::White) {
        if (castlingRights.whiteKingSide) {
            if (boardState[5][7].type == PieceType::None
                && boardState[6][7].type == PieceType::None)
                kingMoves[6][7] = MoveType::CastlingKingSide;
        }
        if (castlingRights.whiteQueenSide) {
            if (boardState[1][7].type == PieceType::None
                && boardState[2][7].type == PieceType::None
                && boardState[3][7].type == PieceType::None)
                kingMoves[2][7] = MoveType::CastlingQueenSide;
        }
    }
    // Black Castling Moves
    else {
        if (castlingRights.blackKingSide) {
            if (boardState[5][0].type == PieceType::None
                && boardState[6][0].type == PieceType::None)
                kingMoves[6][0] = MoveType::CastlingKingSide;
        }
        if (castlingRights.blackQueenSide) {
            if (boardState[1][0].type == PieceType::None
                && boardState[2][0].type == PieceType::None
                && boardState[3][0].type == PieceType::None)
                kingMoves[2][0] = MoveType::CastlingQueenSide;
        }
    }
    return kingMoves;
}

std::array<std::array<MoveType, 8>, 8> moveGeneration::generateQueenMoves(
    Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState) {
    
    return generateMoves(moveOrigin, TeamColor, boardState, {
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left,
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 7);
}

std::array<std::array<MoveType, 8>, 8> moveGeneration::generateRookMoves(
    Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState) {
    
    return generateMoves(moveOrigin, TeamColor, boardState, {
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left}, 7);
}

std::array<std::array<MoveType, 8>, 8> moveGeneration::generateBishopMoves(
    Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState) {
    
    return generateMoves(moveOrigin, TeamColor, boardState, {
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 7);
}

std::array<std::array<MoveType, 8>, 8> moveGeneration::generateKnightMoves(
    Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState) {
    
    return generateMoves(moveOrigin, TeamColor, boardState, {
        {2,1}, {2,-1}, {1,2}, {-1,2}, {-2,1}, {-2,-1}, {1,-2}, {-1,-2}}, 1);
}

std::array<std::array<MoveType, 8>, 8> moveGeneration::generatePawnMoves(
    Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState,
    Vector2Int enPassantTargetSquare, bool isFirstMove) {
    
    std::array<std::array<MoveType, 8>, 8> moves;
    clearMoves(moves);

    // Pawns move in different directions depending on their team
    int forwardDirection = (TeamColor == TeamColor::White) ? -1 : 1;

    // Normal pawn movement direction is simply up.
    std::vector<Vector2Int> directions{Vector2Int::Up * forwardDirection};

    // Double Pawn Push (On first turn only)
    if (isFirstMove)
        directions.push_back(Vector2Int{0, 2} * forwardDirection);

    // Pawns attack diagonally
    std::vector<Vector2Int> attackDirections{
        Vector2Int::UpLeft * forwardDirection, Vector2Int::UpRight * forwardDirection};

    // Movement
    for (Vector2Int direction : directions) {
        Vector2Int targetSquare = moveOrigin + direction;

        if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
            break;

        if (boardState[targetSquare.x][targetSquare.y].type == PieceType::None) {
            // Check if it's a promotion
            if (targetSquare.y == 0 || targetSquare.y == 7) {
                moves[targetSquare.x][targetSquare.y] = MoveType::Promotion;
            }
            else {
                moves[targetSquare.x][targetSquare.y] = MoveType::Move;
            }
        }
        else {
            break; // Pawns are blocked by any piece in a target movement square.
        }
    }
    // Attack
    for (Vector2Int attackDirection : attackDirections) {
        Vector2Int targetSquare = moveOrigin + attackDirection;

        if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
            continue;

        // There is a piece in this square
        if (boardState[targetSquare.x][targetSquare.y].type != PieceType::None) {
            if (boardState[targetSquare.x][targetSquare.y].team != TeamColor)
                // Check if it's a promotion
                if (targetSquare.y == 0 || targetSquare.y == 7) {
                    moves[targetSquare.x][targetSquare.y] = MoveType::CapturePromotion;
                }
                else {
                    moves[targetSquare.x][targetSquare.y] = MoveType::Capture;
                }
            continue;
        }
        // Empty square (Could it be en passant?)
        if (targetSquare == enPassantTargetSquare)
            moves[targetSquare.x][targetSquare.y] = MoveType::EnPassant;
    }

    return moves;
}

std::array<std::array<MoveType, 8>, 8> moveGeneration::generateMoves(
    Vector2Int moveOrigin, TeamColor TeamColor,
    const std::array<std::array<Tile, 8>, 8>& boardState,
    std::vector<Vector2Int> directions, int maxSteps) {

    std::array<std::array<MoveType, 8>, 8> moves;
    clearMoves(moves);

    for (Vector2Int direction : directions) {
        for (int steps = 1; steps <= maxSteps; steps++) {
            Vector2Int targetSquare = moveOrigin + direction * steps;

            if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
                break;

            if (boardState[targetSquare.x][targetSquare.y].type != PieceType::None) {
                if (boardState[targetSquare.x][targetSquare.y].team != TeamColor)
                    moves[targetSquare.x][targetSquare.y] = MoveType::Capture;
                break;
            }

            moves[targetSquare.x][targetSquare.y] = MoveType::Move;
        }
    }

    return moves;
}

void moveGeneration::clearMoves(std::array<std::array<MoveType, 8>, 8>& moves) {
    for (auto& rank : moves) rank.fill(MoveType::None);
}

// Find all of the squares attacked by the oponent. 
// If ignoreKing is set to true, the attacked squares of sliding pieces will ignore the king.
// This is necessary for finding safe king squares since the king cannot retreat to the squares it was blocking. (X-ray)
std::array<std::array<bool, 8>, 8> moveGeneration::getAttackedSquares(bool ignoreKing, TeamColor playerTeam,
    const std::array<std::array<Tile, 8>, 8>& boardState, const CastlingRights& castlingRights) {
    // Store attacked squares
    std::array<std::array<bool, 8>, 8> attackedSquares;

    // Set attacked squares to none
    for (auto& rank : attackedSquares)
        for (auto& square : rank)
            square = false;

    // Store an adjusted board state (In case we are ignoring the king)
    std::array<std::array<Tile, 8>, 8> adjustedBoardState = boardState;
    if (ignoreKing) {
        for (int r = 0; r < 8; r++)
            for (int f = 0; f < 8; f++)
                if (boardState[f][r].team == playerTeam && boardState[f][r].type == PieceType::King)
                    adjustedBoardState[f][r] = {PieceType::None, TeamColor::None};
    }

    // Find every enemy piece
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Tile& cell = adjustedBoardState[file][rank];
            // This cell is empty, do nothing
            if (cell.type == PieceType::None || cell.team == playerTeam)
                continue;

            // This cell is a pawn. Pawns attack diagonally so they require special treatment
            // We don't need an exception for en passant since it doesn't change the attack shape (Diagonal)
            if (cell.type == PieceType::Pawn) {
                int direction = (cell.team == TeamColor::White) ? -1 : 1;
                int attackRank = rank + direction;
                if (attackRank >= 0 && attackRank < 8) {
                    if (file - 1 >= 0) attackedSquares[file - 1][attackRank] = true;
                    if (file + 1 < 8)  attackedSquares[file + 1][attackRank] = true;
                }
                continue;
            }

            // This cell is a piece other than a pawn, we can use move generation to find its attack squares
            std::array<std::array<MoveType, 8>, 8> moves =
                moveGeneration::generateMovesForPiece(cell, Vector2Int(file, rank), adjustedBoardState, castlingRights);

            // Only add valid attack moves to the attacked squares (Not Empty or Castling)
            // We don't need to account for promotion since forward pawn moves don't attack anyway.
            for (int r = 0; r < 8; r++) {
                for (int f = 0; f < 8; f++) {
                    MoveType m = moves[f][r];
                    if (m == MoveType::None
                        || m == MoveType::CastlingKingSide
                        || m == MoveType::CastlingQueenSide)
                        continue;

                    attackedSquares[f][r] = true;
                }
            }
        }
    }

    return attackedSquares;
}