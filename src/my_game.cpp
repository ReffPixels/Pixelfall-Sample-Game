// Defines the loop of this specific application.

#include "my_game.h"
// Standard Library
#include <iostream>

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
    fenParser.getPiecesFromFEN(currentBoardFEN);

    return true;
}

// Called every frame
void MyGame::onUpdate() {
    appWindow->setWindowTitle("Chess | FPS: " + std::to_string(appClock->getFPS()));
}

// Handles draw calls
void MyGame::onRender() {
    // Background
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getLogicalSize().x, appWindow->getLogicalSize().y),
        Color::fromHexcode("#333")
    );

    // Chess board
    Vector2 squareSize{100.0f, 100.0f};
    Vector2 boardSize{squareSize.x * 8, squareSize.y * 8};
    Color whiteSquareColor{Color::fromHexcode("#edd6b0")};
    Color blackSquareColor{Color::fromHexcode("#b88762")};

    // Center board
    Vector2 boardPosition{
        (appWindow->getLogicalSize().x - boardSize.x) / 2,
        (appWindow->getLogicalSize().y - boardSize.y) / 2
    };

    // Draw board
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            painter->drawRectangle(
                {boardPosition.x + (file * squareSize.x), boardPosition.y + (rank * squareSize.y)},
                squareSize,
                // If the square is odd, draw white. If it's even, draw black.
                ((file + rank + 1) % 2 == 0) ? blackSquareColor : whiteSquareColor
            );
        }
    }

    // Draw pieces
    fenParser.drawPieces(boardPosition, squareSize, *painter, *textureCache, projectPath);
}
