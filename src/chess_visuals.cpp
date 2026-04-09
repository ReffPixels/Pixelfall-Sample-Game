#include "chess_visuals.h"

void ChessVisuals::draw() {
    // Draw Board
    board.draw(*painter);

    // Draw Selected Highlight
    highlightSelected();

    // Draw Last Move
    highlightLastMove();

    // Check if user is in drag and drop mode
    bool dragAndDrop{inputState == InputState::PieceSelected && appInput->isMouseButtonDown(MouseButton::Left)};
    // Drag and Drop Visuals (Behind pieces)
    pieces.setHideSelectedPiece(dragAndDrop);
    if (dragAndDrop) highlightHoveredSquare();

    // Draw pieces
    pieces.drawPieces(boardState, board.getPosition(), board.getTileSize(),
        board.getTileSize(), *painter, Vector2::Zero, selectedPiecePosition);

    // Drag and Drop Visuals (On top of pieces)
    if (dragAndDrop) {
        pieceFollowCursor(
            boardState[selectedPiecePosition.x][selectedPiecePosition.y].type,
            boardState[selectedPiecePosition.x][selectedPiecePosition.y].team,
            dragAndDropPivot
        );
    }
}

// Draw a highlight on the square of the selected piece
void ChessVisuals::highlightSelected(ChessBoard& board, Vector2Int& selectedPiecePosition, Painter& painter) {
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
void ChessVisuals::highlightLastMove(ChessBoard& board, Vector2Int& lastMoveOrigin,
    Vector2Int& lastMoveTarget, Painter& painter) {
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

void ChessVisuals::updateDragAndDropPoint(Vector2& cursorPos, ChessBoard& board, Vector2Int& selectedPiecePosition) {
    dragAndDropPivot = {cursorPos - (board.getPosition() +
        Vector2{(float)selectedPiecePosition.x, (float)selectedPiecePosition.y} * board.getTileSize())};
}