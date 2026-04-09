// Defines behaviours of the chess game

#pragma once
// Chess
#include "chess_game.h"
// Standard Library
#include <memory>
#include <array>
#include <vector>

class ChessVisuals {
public:
    // Methods
    void highlightSelected(ChessBoard& board, Vector2Int& selectedPiecePosition, Painter& painter);
    void highlightLastMove(ChessBoard& board, Vector2Int& lastMoveOrigin,
        Vector2Int& lastMoveTarget, Painter& painter);
    void highlightHoveredSquare(Vector2& cursorPos, ChessBoard& board, Painter& painter);
    void pieceFollowCursor(Vector2& cursorPos, ChessPieces& pieces, ChessBoard& board,
        PieceInfo pieceInfo, Painter& painter, Vector2 offset = Vector2::Zero);
    void updateDragAndDropPoint(Vector2& cursorPos, ChessBoard& board, Vector2Int& selectedPiecePosition);

private:
    
    Vector2 dragAndDropPivot{Vector2::Zero}; // Used to attach the piece to the cursor
};