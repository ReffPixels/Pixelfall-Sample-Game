// Defines the loop of this specific application.

#include "chess.h"
// Standard Library
#include <iostream>
#include <algorithm>

// Create Game
PIXELFALL_APPLICATION(Chess);

// Called on the first frame
bool Chess::onStart() {
    // Set Metadata
    appWindow->setWindowTitle("Chess");
    appTitle = "Chess";
    appVersion = "0.1";
    appIdentifier = "chess";

    // Get initial state of board
    state.setupFromFEN();

    return true;
}

// Called every frame
void Chess::onUpdate() {

    // Track FPS in window title
    appWindow->setWindowTitle("Chess | FPS: " + std::to_string(appClock->getFPS()));

    // Store Cursor Position
    cursorPos = appWindow->physicalToLogical(appInput->getMousePhysicalPosition());

    // Center Board
    board.setPosition((appWindow->getLogicalSize() / 2) - (board.getTileSize() * 4));

    // Handle board interaction
    if (board.isBoardOnHover(cursorPos)) {
        Vector2Int hoveredSquare = ChessPieces::getPosFromNotation(board.getSquareOnHover(cursorPos));
        if (appInput->isMouseButtonPressed(MouseButton::Left)) {
            state.onBoardPressed(hoveredSquare);
            // Capture drag pivot at the moment of selection
            if (state.getInputState() == InputState::PieceSelected)
                dragAndDropPivot = visuals.computeDragPivot(cursorPos, board, state.getSelectedPiecePosition());
        }
        if (appInput->isMouseButtonReleased(MouseButton::Left)) state.onBoardReleased(hoveredSquare);
    }
}

// Called at the end of each frame (Handles rendering)
void Chess::onRender() {
    // Draw Background
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getLogicalSize().x, appWindow->getLogicalSize().y),
        Color::fromHexcode("#212121")
    );

    // Draw Board
    board.draw(*painter);

    // Draw Highlights
    Vector2Int selPos = state.getSelectedPiecePosition();
    Vector2Int lastOrigin = state.getLastMoveOrigin();
    Vector2Int lastTarget = state.getLastMoveTarget();
    visuals.highlightLastMove(board, lastOrigin, lastTarget, *painter);
    visuals.highlightSelected(board, selPos, *painter);

    bool dragAndDrop = state.getInputState() == InputState::PieceSelected
        && appInput->isMouseButtonDown(MouseButton::Left);

    // Draw Pieces (hide selected piece during drag to avoid duplication)
    pieces.setHideSelectedPiece(dragAndDrop);
    pieces.drawPieces(state.getBoardState(), board.getPosition(), board.getTileSize(),
        board.getTileSize(), *painter, Vector2::Zero, selPos);

    // Drag and Drop Visuals (piece following cursor)
    if (dragAndDrop) {
        visuals.pieceFollowCursor(cursorPos, pieces, board,
            state.getBoardState()[selPos.x][selPos.y], *painter, dragAndDropPivot);
    }
}