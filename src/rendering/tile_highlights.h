// Handles all chess rendering: board highlights, pieces, and drag-and-drop visuals.

#pragma once
// Chess
#include "config/chess_config.h"
#include "move_generation.h"
#include "game_objects/chess_board.h"

namespace TileHighlights {
    void highlightSelected(ChessBoard& board, Vector2Int selPiecePosition, Painter& painter);
    void highlightLastMove(ChessBoard& board, Vector2Int lastMoveOrigin,
        Vector2Int lastMoveTarget, Painter& painter);
    void highlightHoveredSquare(Vector2& cursorPos, ChessBoard& board,
        Painter& painter, Vector2Int selPiecePosition);
    void highlightValidMoves(std::array<std::array<MoveType, 8>, 8> validMoves,
        ChessBoard& board, Painter& painter);

    // Debug
    void highlightAttackedSquares(std::array<std::array<bool, 8>, 8> attackedSquares,
        ChessBoard& board, Painter& painter, Color color = Color::fromHexcode("#ff000088"));
};

// [TODO] Lerp animation piece when clicking instead of dragging