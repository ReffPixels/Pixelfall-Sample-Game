// [TODO] describe
#include "chess_moves.h"

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateKingMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {

    // Store Moves (Start empty)
    std::array<std::array<MoveType, 8>, 8> kingMoves;
    clearMoves(kingMoves);

    // Directions a king can move
    Vector2Int moveDirections[8]{
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left,
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft};

    // Loop through every move in every direction
    for (Vector2Int direction : moveDirections) {
        // Move once (Maximum move length for a king)
        Vector2Int targetSquare = moveOrigin + direction;

        // There are no more squares in this direction
        if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
            continue;

        // There is a piece in the target square
        if (boardState[targetSquare.x][targetSquare.y].type != PieceType::None) {
            if (boardState[targetSquare.x][targetSquare.y].team != pieceTeam) {
                // Add enemy square to the move list, then stop searching in this direction
                kingMoves[targetSquare.x][targetSquare.y] = MoveType::Capture;
                continue;
            }
            // It's an ally piece
            else {
                continue; // Stop searching in this direction
            }
        }
        // There is no piece in the target square
        else {
            kingMoves[targetSquare.x][targetSquare.y] = MoveType::Move;
        }
    }

    return kingMoves;
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateQueenMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {

    // Store Moves (Start empty)
    std::array<std::array<MoveType, 8>, 8> queenMoves;
    clearMoves(queenMoves);

    // Directions a queen can move
    Vector2Int moveDirections[8]{
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left,
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft};

    // Loop through every move in every direction
    for (Vector2Int direction : moveDirections) {
        // Move up to 7 times (Maximum move length for a queen)
        for (int steps = 1; steps < 8; steps++) {

            Vector2Int targetSquare = moveOrigin + direction * steps;

            // There are no more squares in this direction
            if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
                break;

            // There is a piece in the target square
            if (boardState[targetSquare.x][targetSquare.y].type != PieceType::None) {
                if (boardState[targetSquare.x][targetSquare.y].team != pieceTeam) {
                    // Add enemy square to the move list, then stop searching in this direction
                    queenMoves[targetSquare.x][targetSquare.y] = MoveType::Capture;
                    break;
                }
                // It's an ally piece
                else {
                    break; // Stop searching in this direction
                }
            }
            // There is no piece in the target square
            else {
                queenMoves[targetSquare.x][targetSquare.y] = MoveType::Move;
            }
        }
    }

    return queenMoves;
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateRookMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {

    // Store Moves (Start empty)
    std::array<std::array<MoveType, 8>, 8> rookMoves;
    clearMoves(rookMoves);

    // Directions a rook can move
    Vector2Int moveDirections[4]{
        Vector2Int::Up, Vector2Int::Right, Vector2Int::Down, Vector2Int::Left};

    // Loop through every move in every direction
    for (Vector2Int direction : moveDirections) {
        // Move up to 7 times (Maximum move length for a rook)
        for (int steps = 1; steps < 8; steps++) {
            
            Vector2Int targetSquare = moveOrigin + direction * steps;

            // There are no more squares in this direction
            if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
                break;

            // There is a piece in the target square
            if (boardState[targetSquare.x][targetSquare.y].type != PieceType::None) {
                if (boardState[targetSquare.x][targetSquare.y].team != pieceTeam) {
                    // Add enemy square to the move list, then stop searching in this direction
                    rookMoves[targetSquare.x][targetSquare.y] = MoveType::Capture;
                    break;
                }
                // It's an ally piece
                else {
                    break; // Stop searching in this direction
                }
            }
            // There is no piece in the target square
            else {
                rookMoves[targetSquare.x][targetSquare.y] = MoveType::Move;
            }
        }
    }

    return rookMoves;
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateBishopMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {

    // Store Moves (Start empty)
    std::array<std::array<MoveType, 8>, 8> bishopMoves;
    clearMoves(bishopMoves);

    // Directions a bishop can move
    Vector2Int moveDirections[4]{
        Vector2Int::UpRight, Vector2Int::DownRight, Vector2Int::DownLeft, Vector2Int::UpLeft};

    // Loop through every move in every direction
    for (Vector2Int direction : moveDirections) {
        // Move up to 7 times (Maximum move length for a bishop)
        for (int steps = 1; steps < 8; steps++) {

            Vector2Int targetSquare = moveOrigin + direction * steps;

            // There are no more squares in this direction
            if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
                break;

            // There is a piece in the target square
            if (boardState[targetSquare.x][targetSquare.y].type != PieceType::None) {
                if (boardState[targetSquare.x][targetSquare.y].team != pieceTeam) {
                    // Add enemy square to the move list, then stop searching in this direction
                    bishopMoves[targetSquare.x][targetSquare.y] = MoveType::Capture;
                    break;
                }
                // It's an ally piece
                else {
                    break; // Stop searching in this direction
                }
            }
            // There is no piece in the target square
            else {
                bishopMoves[targetSquare.x][targetSquare.y] = MoveType::Move;
            }
        }
    }

    return bishopMoves;
}

std::array<std::array<MoveType, 8>, 8> ChessMoves::generateKnightMoves(
    Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {

    // Store Moves (Start empty)
    std::array<std::array<MoveType, 8>, 8> knightMoves;
    clearMoves(knightMoves);

    // Directions a knight can move
    Vector2Int moveDirections[8]{
        Vector2Int{2, 1}, Vector2Int{2, -1}, Vector2Int{1, 2}, Vector2Int{-1, 2},
        Vector2Int{-2, 1}, Vector2Int{-2, -1}, Vector2Int{1, -2}, Vector2Int{-1, -2}};

    // Loop through every move in every direction
    for (Vector2Int direction : moveDirections) {
        // Move once (Maximum move length for a knight)
        Vector2Int targetSquare = moveOrigin + direction;

        // There are no more squares in this direction
        if (targetSquare.x < 0 || targetSquare.x > 7 || targetSquare.y < 0 || targetSquare.y > 7)
            continue;

        // There is a piece in the target square
        if (boardState[targetSquare.x][targetSquare.y].type != PieceType::None) {
            if (boardState[targetSquare.x][targetSquare.y].team != pieceTeam) {
                // Add enemy square to the move list, then stop searching in this direction
                knightMoves[targetSquare.x][targetSquare.y] = MoveType::Capture;
                continue;
            }
            // It's an ally piece
            else {
                continue; // Stop searching in this direction
            }
        }
        // There is no piece in the target square
        else {
            knightMoves[targetSquare.x][targetSquare.y] = MoveType::Move;
        }
    }

    return knightMoves;
}

void ChessMoves::clearMoves(std::array<std::array<MoveType, 8>, 8>& moves) {
    
    // Set all moves to none
    for (auto& rank : moves)
        for (auto& move : rank)
            move = MoveType::None;
}