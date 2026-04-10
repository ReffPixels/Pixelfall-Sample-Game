// [TODO] describe
#include "chess_moves.h"

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateKingMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left,
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 1);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateQueenMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left,
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 7);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateRookMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left}, 7);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateBishopMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft}, 7);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateKnightMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {
    
    return generateMoves(moveOrigin, pieceTeam, boardState, {
        {2,1}, {2,-1}, {1,2}, {-1,2}, {-2,1}, {-2,-1}, {1,-2}, {-1,-2}}, 1);
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generatePawnMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState,
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
            moves[targetSquare.x][targetSquare.y] = MoveType::Move;
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
                moves[targetSquare.x][targetSquare.y] = MoveType::Capture;
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
    std::array<std::array<PieceInfo, 8>, 8> boardState,
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