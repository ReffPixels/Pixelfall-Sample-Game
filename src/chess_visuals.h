// Handles all chess rendering: board highlights, pieces, and drag-and-drop visuals.

#pragma once
// Chess
#include "game_objects/chess_board.h"
#include "game_objects/chess_pieces.h"
#include "chess_moves.h"

class ChessVisuals {
public:
    // Square Highlights
    void highlightSelected(ChessBoard& board, Vector2Int selPiecePosition, Painter& painter);
    void highlightLastMove(ChessBoard& board, Vector2Int lastMoveOrigin,
        Vector2Int lastMoveTarget, Painter& painter);
    void highlightHoveredSquare(Vector2& cursorPos, ChessBoard& board,
        Painter& painter, Vector2Int selPiecePosition);

    // Valid Moves
    void highlightValidMoves(std::array<std::array<MoveType, 8>, 8> validMoves,
        ChessBoard& board, Painter& painter);

    // Debug
    void highlightAttackedSquares(std::array<std::array<bool, 8>, 8> attackedSquares,
        ChessBoard& board, Painter& painter);

    // Cursor
    void pieceFollowCursor(Vector2& cursorPos, ChessPieces& pieces, ChessBoard& board,
        PieceInfo pieceInfo, Painter& painter, Vector2 offset = Vector2::Zero);
    Vector2 computeDragPivot(Vector2& cursorPos, ChessBoard& board, Vector2Int selPiecePosition);
};