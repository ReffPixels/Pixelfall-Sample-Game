// Defines the loop of this specific application.

#include "my_game.h"
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
    game.board.setPosition((appWindow->getLogicalSize() / 2) - (game.board.getTileSize() * 4));

    // Handle board interaction
    if (game.board.isBoardOnHover(cursorPos)) {
        Vector2Int hoveredSquare = ChessPieces::getPosFromNotation(game.board.getSquareOnHover(cursorPos));
        if (appInput->isMouseButtonPressed(MouseButton::Left))  game.onBoardPressed(hoveredSquare);
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

    
}