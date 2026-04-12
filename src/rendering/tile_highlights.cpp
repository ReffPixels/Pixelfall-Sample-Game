// Implementation for tile_highlights.h

#include "tile_highlights.h"
// Standard Library
#include <algorithm>

// Draw a highlight on the square of the selected piece
void TileHighlights::highlightSelected(ChessBoard& board, Vector2Int selPiecePosition, Painter& painter) {
    if (selPiecePosition != Vector2Int{-1, -1}) {
        painter.drawRectangle(
            board.getPosition()
            + Vector2(selPiecePosition.x * board.getTileSize().x,
                board.getRankByDirection(selPiecePosition.y) * board.getTileSize().y),
            board.getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
    }
}

// Draw a highlight on the start and end squares of the last move
void TileHighlights::highlightLastMove(ChessBoard& board, Vector2Int lastMoveOrigin,
    Vector2Int lastMoveTarget, Painter& painter) {
    if (lastMoveOrigin != Vector2Int{-1, -1} && lastMoveTarget != Vector2Int{-1, -1}) {
        painter.drawRectangle(
            board.getPosition()
            + Vector2(lastMoveOrigin.x * board.getTileSize().x,
                board.getRankByDirection(lastMoveOrigin.y) * board.getTileSize().y),
            board.getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
        painter.drawRectangle(
            board.getPosition()
            + Vector2(lastMoveTarget.x * board.getTileSize().x,
                board.getRankByDirection(lastMoveTarget.y) * board.getTileSize().y),
            board.getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
    }
}

// Draw a highlight on the square that the cursor is hovering
void TileHighlights::highlightHoveredSquare(Vector2& cursorPos, ChessBoard& board,
    Painter& painter, Vector2Int selPiecePosition) {
    // Snap to grid position
    Vector2Int gridPos = board.getTileOnHover(cursorPos);
    if (gridPos == selPiecePosition) return; // Don't draw highlight over origin square

    Vector2 snappedPositionInBoard{
        std::clamp(board.getPosition().x + (float)gridPos.x * board.getTileSize().x,
            board.getPosition().x, board.getPosition().x + board.getTileSize().x * 7),
        std::clamp(board.getPosition().y + board.getRankByDirection(gridPos.y) * board.getTileSize().y,
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

// Draw a circle at every move location provided
void TileHighlights::highlightMoves(std::array<std::array<MoveType, 8>, 8> validMoves, ChessBoard& board, Painter& painter) {
    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++)
            // It's a normal move or it's castling
            if (validMoves[file][rank] == MoveType::Move
                || validMoves[file][rank] == MoveType::CastlingKingSide
                || validMoves[file][rank] == MoveType::CastlingQueenSide
                || validMoves[file][rank] == MoveType::Promotion)

                painter.drawCircle(
                    Vector2(board.getPosition().x + (float)file * board.getTileSize().x + board.getTileSize().x / 2,
                        board.getPosition().y + (float)board.getRankByDirection(rank) * board.getTileSize().y + board.getTileSize().y / 2),
                    board.getTileSize().x * 0.15f,
                    Color::fromHexcode("#0000002a")
                );
            // It's a capture
            else if ((validMoves[file][rank] == MoveType::Capture)
                || (validMoves[file][rank] == MoveType::EnPassant)
                || (validMoves[file][rank] == MoveType::CapturePromotion)) {
                painter.drawCircleHollow(
                    Vector2(board.getPosition().x + (float)file * board.getTileSize().x + board.getTileSize().x / 2,
                        board.getPosition().y + (float)board.getRankByDirection(rank) * board.getTileSize().y + board.getTileSize().y / 2),
                    board.getTileSize().x * 0.45f,
                    board.getTileSize().x * 0.3f,
                    Color::fromHexcode("#c5131355")
                );
            }
}

// Draw a red highlight on every square attacked by the opponent
void TileHighlights::highlightAttackedSquares(std::array<std::array<bool, 8>, 8> attackedSquares,
    ChessBoard& board, Painter& painter, Color color) {
    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++)
            if (attackedSquares[file][rank])
                painter.drawRectangle(
                    board.getPosition()
                    + Vector2(file * board.getTileSize().x, board.getRankByDirection(rank) * board.getTileSize().y),
                    board.getTileSize(),
                    color
                );
}