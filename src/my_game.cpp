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

    // Create board
    board = std::make_unique<ChessBoard>(appWindow->getLogicalSize()/2);

    // Get initial state of board
    piecesInfo = fenParser.getPiecesFromFEN(currentBoardFEN);

    return true;
}

// Called every frame
void MyGame::onUpdate() {
    appWindow->setWindowTitle("Chess | FPS: " + std::to_string(appClock->getFPS()));

    if (appInput->isMouseButtonPressed(MouseButton::Left)) {
        //pieceTestPosition = appWindow->physicalToLogical(appInput->getMousePhysicalPosition());
        pieceTestPosition = {4, 4};
        movePiece(piecesInfo[5], "g4");
    }
}

// Handles draw calls
void MyGame::onRender() {
    // Background
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getLogicalSize().x, appWindow->getLogicalSize().y),
        Color::fromHexcode("#333")
    );

    // Draw Board
    board->draw(*painter);

    // Draw pieces
    pieces.drawPieces(piecesInfo, board->getPosition(), board->getTileSize(), board->getTileSize(), *painter);
}

void MyGame::movePiece(PieceInfo& piece, std::string newPos) {
    piece.position = pieces.getPosFromNotation(newPos);
}