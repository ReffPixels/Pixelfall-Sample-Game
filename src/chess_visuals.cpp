// Implementation for chess_visuals.h

#include "chess_visuals.h"
// Standard Library
#include <algorithm>

// Draw a highlight on the square of the selected piece
void ChessVisuals::highlightSelected(ChessBoard& board, Vector2Int selectedPiecePosition, Painter& painter) {
    if (selectedPiecePosition != Vector2Int{-1, -1}) {
        painter.drawRectangle(
            board.getPosition()
            + Vector2(selectedPiecePosition.x * board.getTileSize().x,
                selectedPiecePosition.y * board.getTileSize().y),
            board.getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
    }
}

// Draw a highlight on the start and end squares of the last move
void ChessVisuals::highlightLastMove(ChessBoard& board, Vector2Int lastMoveOrigin,
    Vector2Int lastMoveTarget, Painter& painter) {
    if (lastMoveOrigin != Vector2Int{-1, -1} && lastMoveTarget != Vector2Int{-1, -1}) {
        painter.drawRectangle(
            board.getPosition()
            + Vector2(lastMoveOrigin.x * board.getTileSize().x,
                lastMoveOrigin.y * board.getTileSize().y),
            board.getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
        painter.drawRectangle(
            board.getPosition()
            + Vector2(lastMoveTarget.x * board.getTileSize().x,
                lastMoveTarget.y * board.getTileSize().y),
            board.getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
    }
}

// Draw a highlight on the square that the cursor is hovering
void ChessVisuals::highlightHoveredSquare(Vector2& cursorPos, ChessBoard& board, Painter& painter) {
    // Snap to grid position
    Vector2Int gridPos = ChessPieces::getPosFromNotation(board.getSquareOnHover(cursorPos));
    Vector2 snappedPositionInBoard{
        std::clamp(board.getPosition().x + (float)gridPos.x * board.getTileSize().x,
            board.getPosition().x, board.getPosition().x + board.getTileSize().x * 7),
        std::clamp(board.getPosition().y + gridPos.y * board.getTileSize().y,
            board.getPosition().y, board.getPosition().y + board.getTileSize().y * 7)
    };

    // Draw highlight border
    painter.drawRectangleHollow(
        snappedPositionInBoard,
        board.getTileSize(),
        board.getTileSize() * 0.9f,
        Color::fromHexcode("#ffffffbd")
    );
}

// Attach a piece visual to the cursor
void ChessVisuals::pieceFollowCursor(Vector2& cursorPos, ChessPieces& pieces, ChessBoard& board,
    PieceInfo pieceInfo, Painter& painter, Vector2 offset) {
    // Clamp cursor position to inside the board
    Vector2 positionInBoard{
        std::clamp(cursorPos.x,
            board.getPosition().x, board.getPosition().x + board.getTileSize().x * 8),
        std::clamp(cursorPos.y,
            board.getPosition().y, board.getPosition().y + board.getTileSize().y * 8)
    };

    pieces.drawFree(
        pieceInfo.type, pieceInfo.team,
        positionInBoard - offset, board.getTileSize(), painter);
}

// Computes the pivot offset so the dragged piece stays attached at the grab point
Vector2 ChessVisuals::computeDragPivot(Vector2& cursorPos, ChessBoard& board, Vector2Int selectedPiecePosition) {
    return {cursorPos - (board.getPosition() +
        Vector2{(float)selectedPiecePosition.x, (float)selectedPiecePosition.y} * board.getTileSize())};
}
