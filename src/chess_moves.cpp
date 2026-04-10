// [TODO] describe
#include "chess_moves.h"

void ChessMoves::findLegalMovesForPiece(std::array<std::array<MoveType, 8>, 8>& moves,
    PieceInfo piece, Vector2Int position, const std::array<std::array<PieceInfo, 8>, 8>& boardState,
    CastlingRights& castlingRights, Vector2Int enPassantTargetSquare) {

    // Iterate through every pseudo legal move
    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            if (moves[f][r] == MoveType::None)
                continue; // Not a valid move, keep looking

            // Create a board that represents the next turn if this move was made 
            std::array<std::array<PieceInfo, 8>, 8> simulatedBoard = boardState;
            simulatedBoard[position.x][position.y] = {PieceType::None, PieceTeam::None};
            simulatedBoard[f][r] = piece;

            // En passant is an exception because the captured pawn is in a different tile than the moved pawn 
            // This is the only move where this happens. We need to clear the pawn origin AND the captured pawn.
            if (moves[f][r] == MoveType::EnPassant)
                simulatedBoard[f][position.y] = {PieceType::None, PieceTeam::None};

            // Find attacked squares in simulated board
            std::array<std::array<bool, 8>, 8> unsafeSquares =
                ChessState::getAttackedSquares(true, piece.team, simulatedBoard, castlingRights);

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
                if ((piece.team == PieceTeam::White && (
                    unsafeSquares[4][7] || unsafeSquares[5][7] || unsafeSquares[6][7])
                        || piece.team == PieceTeam::Black && (
                        unsafeSquares[4][0] || unsafeSquares[5][0] || unsafeSquares[6][0])))
                    // Remove illegal move
                    moves[f][r] = MoveType::None;
            if (moves[f][r] == MoveType::CastlingQueenSide)
                if ((piece.team == PieceTeam::White && (
                    unsafeSquares[2][7] || unsafeSquares[3][7] || unsafeSquares[4][7])
                    || piece.team == PieceTeam::Black && (
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
std::array<std::array<MoveType, 8>, 8> ChessMoves::generateMovesForPiece(
    PieceInfo piece, Vector2Int position, const std::array<std::array<PieceInfo, 8>, 8>& boardState,
    CastlingRights& castlingRights, Vector2Int enPassantTargetSquare) {

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
        bool isFirstMove = (piece.team == PieceTeam::White) ? (position.y == 6) : (position.y == 1);
        return generatePawnMoves(position, piece.team, boardState, enPassantTargetSquare, isFirstMove);
    }
    default:
        std::array<std::array<MoveType, 8>, 8> empty{};
        return empty;
    }
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateKingMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState,
    CastlingRights& castlingRights) {

    // Normal Moves
    std::array<std::array<MoveType, 8>, 8> kingMoves =
        generateMoves(moveOrigin, pieceTeam, boardState, {
            Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left,
            Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 1);

    // White Castling Moves
    if (pieceTeam == PieceTeam::White) {
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

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateQueenMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left,
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 7);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateRookMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left}, 7);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateBishopMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 7);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateKnightMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        {2,1}, {2,-1}, {1,2}, {-1,2}, {-2,1}, {-2,-1}, {1,-2}, {-1,-2}}, 1);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generatePawnMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState,
    Vector2Int enPassantTargetSquare, bool isFirstMove) {
    
    std::array<std::array<MoveType, 8>, 8> moves;
    clearMoves(moves);

    // Pawns move in different directions depending on their team
    int forwardDirection = (pieceTeam == PieceTeam::White) ? -1 : 1;

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
            if (boardState[targetSquare.x][targetSquare.y].team != pieceTeam)
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

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam,
    const std::array<std::array<PieceInfo, 8>, 8>& boardState,
    std::vector<Vector2Int> directions, int maxSteps) {

    std::array<std::array<MoveType, 8>, 8> moves;
    clearMoves(moves);

    for (Vector2Int direction : directions) {
        for (int steps = 1; steps <= maxSteps; steps++) {
            Vector2Int targetSquare = moveOrigin + direction * steps;

            if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
                break;

            if (boardState[targetSquare.x][targetSquare.y].type != PieceType::None) {
                if (boardState[targetSquare.x][targetSquare.y].team != pieceTeam)
                    moves[targetSquare.x][targetSquare.y] = MoveType::Capture;
                break;
            }

            moves[targetSquare.x][targetSquare.y] = MoveType::Move;
        }
    }

    return moves;
}

void ChessMoves::clearMoves(std::array<std::array<MoveType, 8>, 8>& moves) {
    
    // Set all moves to none
    for (auto& rank : moves)
        for (auto& move : rank)
            move = MoveType::None;
}