// Defines the loop of this specific application.

#include "game.h"
// Standard Library
#include <iostream>
#include <algorithm>

// Create Game
PIXELFALL_APPLICATION(MyGame);

// Called on the first frame
bool MyGame::onStart() {
    // Set Metadata
    appWindow->setWindowTitle("Chess");
    appTitle = "Chess";
    appVersion = "0.1";
    appIdentifier = "chess";

    // Get initial state of board
    game.setupFromFEN();

    return true;
}

// Called every frame
void MyGame::onUpdate() {

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
            game.onBoardPressed(hoveredSquare);
            // Capture drag pivot at the moment of selection
            if (game.getInputState() == InputState::PieceSelected)
                dragAndDropPivot = visuals.computeDragPivot(cursorPos, board, game.getSelectedPiecePosition());
        }
        if (appInput->isMouseButtonReleased(MouseButton::Left)) game.onBoardReleased(hoveredSquare);
    }
}

// Called at the end of each frame (Handles rendering)
void MyGame::onRender() {
    // Draw Background
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getLogicalSize().x, appWindow->getLogicalSize().y),
        Color::fromHexcode("#212121")
    );

    // Draw Board
    board.draw(*painter);

    // Draw Highlights
    Vector2Int selPos = game.getSelectedPiecePosition();
    Vector2Int lastOrigin = game.getLastMoveOrigin();
    Vector2Int lastTarget = game.getLastMoveTarget();
    visuals.highlightLastMove(board, lastOrigin, lastTarget, *painter);
    visuals.highlightSelected(board, selPos, *painter);

    bool dragAndDrop = game.getInputState() == InputState::PieceSelected
        && appInput->isMouseButtonDown(MouseButton::Left);

    // Draw Pieces (hide selected piece during drag to avoid duplication)
    pieces.setHideSelectedPiece(dragAndDrop);
    pieces.drawPieces(game.getBoardState(), board.getPosition(), board.getTileSize(),
        board.getTileSize(), *painter, Vector2::Zero, selPos);

    // Drag and Drop Visuals (piece following cursor)
    if (dragAndDrop) {
        visuals.pieceFollowCursor(cursorPos, pieces, board,
            game.getBoardState()[selPos.x][selPos.y], *painter, dragAndDropPivot);
    }
}