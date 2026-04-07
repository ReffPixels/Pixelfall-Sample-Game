// Defines the loop of this specific application.

#include "my_game.h"
// Standard Library
#include <iostream>
// Chess
#include "config/board_config.h"

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
    Vector2 tileSize{64.0f, 64.0f};  // width:height = 2:1
    Color whiteSquareColor{Color::fromHexcode("#edd6b0")};
    Color blackSquareColor{Color::fromHexcode("#b88762")};

    Vector2 boardPosition{
        board::projectionType == ThemeProjection::Isometric ?
        // Center of the whole board
        Vector2{
            (appWindow->getLogicalSize().x) / 2.0f,
            (appWindow->getLogicalSize().y - (tileSize.y * 8)) / 2.0f}
        :
        // Top Left Corner
        Vector2{
            (appWindow->getLogicalSize().x - (tileSize.x * 8)) / 2,
            (appWindow->getLogicalSize().y - (tileSize.y * 8)) / 2}
    };

    // Draw board
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (board::projectionType == ThemeProjection::Isometric) {
                // Center of this tile in screen space
                float cx = boardPosition.x + (file - rank) * (tileSize.x / 2);
                float cy = boardPosition.y + (file + rank) * (tileSize.y / 2);

                // Four corners of the diamond
                Vector2 top{cx,         cy - (tileSize.y / 2)};
                Vector2 right{cx + (tileSize.x / 2), cy};
                Vector2 bot{cx,         cy + (tileSize.y / 2)};
                Vector2 left{cx - (tileSize.x / 2), cy};

                painter->drawQuad(
                    top, right, bot, left,
                    ((file + rank + 1) % 2 == 0) ? blackSquareColor : whiteSquareColor
                );
            }
            else {
                painter->drawRectangle(
                    {boardPosition.x + (file * tileSize.x), boardPosition.y + (rank * tileSize.y)},
                    tileSize,
                    ((file + rank + 1) % 2 == 0) ? blackSquareColor : whiteSquareColor
                );
            }
        }
    }

    // Draw pieces
    fenParser.drawPieces(boardPosition, tileSize, tileSize, *painter, *textureCache, projectPath);
}
