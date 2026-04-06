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

    pawnTexture = new Texture(projectPath / "assets/image/pieces/white_rook.png");

    myPiece = new ChessPiece{"a1", PieceType::Queen, PieceTeam::Black, projectPath};

    return true;
}

// Called every frame
void MyGame::onUpdate() {
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
    Vector2 squareSize{64.0f, 64.0f};
    Vector2 boardSize{squareSize.x * 8, squareSize.y * 8};
    Color whiteSquareColor{Color::fromHexcode("#edd6b0")};
    Color blackSquareColor{Color::fromHexcode("#b88762")};

    // Center board
    Vector2 boardPosition{
        (appWindow->getLogicalSize().x - boardSize.x) / 2,
        (appWindow->getLogicalSize().y - boardSize.y) / 2
    };

    // Draw board
    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            painter->drawRectangle(
                {boardPosition.x + (column * squareSize.x), boardPosition.y + (row * squareSize.y)},
                squareSize,
                // If the square is odd, draw white. If it's even, draw black.
                ((column + row + 1) % 2 == 0) ? blackSquareColor : whiteSquareColor
            );
        }
    }

    myPiece->draw(*painter, projectPath, boardPosition, squareSize);
}
