// [TODO] describe
#include "chess_moves.h"

std::array<std::array<MoveType, 8>, 8> ChessMoves::GenerateRookMoves(
    Vector2Int moveOrigin, PieceTeam rookTeam, std::array<std::array<PieceInfo, 8>, 8> boardState) {

    // Store Moves
    std::array<std::array<MoveType, 8>, 8> rookMoves;

    // Set all moves to none
    for (auto& rank : rookMoves)
        for (auto& move : rank)
            move = MoveType::None;

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
                if (boardState[targetSquare.x][targetSquare.y].team != rookTeam) {
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