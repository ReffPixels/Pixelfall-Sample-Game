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

    // Create board
    board = std::make_unique<ChessBoard>();

    // Get initial state of board
    piecesInfo = fenParser.getPiecesFromFEN(currentBoardFEN);

    // White plays first
    playerTeam = PieceTeam::White;

    return true;
}

// Called every frame
void MyGame::onUpdate() {
    Vector2 mousePos{appWindow->physicalToLogical(appInput->getMousePhysicalPosition())};

    // Center board
    board->setPosition(
        (appWindow->getLogicalSize() / 2) - (board->getTileSize() * 4)
    );

    appWindow->setWindowTitle("Chess | FPS: " + std::to_string(appClock->getFPS()));

    if (appInput->isMouseButtonPressed(MouseButton::Left)) {
        // Console Debug
        std::cout << board->getSquareOnHover(mousePos) << std::endl;

        // Get square clicked
        std::string hoveredSquare = board->getSquareOnHover(mousePos);

        // Check if there's a piece in that square
        auto it = std::find_if(piecesInfo.begin(), piecesInfo.end(), [&](const PieceInfo& p) {
            return p.position == ChessPieces::getPosFromNotation(hoveredSquare);
            });

        // The Square clicked on has a piece on it
        if (it != piecesInfo.end()) {
            // No piece is currently selected
            if (inputState == Normal) {
                // The piece is on our team
                if (it->team == playerTeam) {
                    selectedPieceInfo = std::distance(piecesInfo.begin(), it);
                    pieces.setIsPieceSelected(true);
                    inputState = InputState::PieceSelected;
                }
            }
            // A piece is already selected
            else if (inputState == PieceSelected) {
                // The piece is NOT on our team
                if (it->team != playerTeam) {
                    // Capture enemy piece
                    int capturedIndex = std::distance(piecesInfo.begin(), it);
                    piecesInfo.erase(it);
                    if (capturedIndex < selectedPieceInfo) selectedPieceInfo--;
                    movePiece(piecesInfo[selectedPieceInfo], board->getSquareOnHover(mousePos));
                    selectedPieceInfo = -1; // [TODO] Detect error when attempting o move a -1 piece
                    pieces.setIsPieceSelected(false);
                    inputState = InputState::Normal;
                    nextTurn();
                }
            }
        }
        // The Square clicked is an empty Square
        else {
            if (inputState == PieceSelected) {
                movePiece(piecesInfo[selectedPieceInfo], board->getSquareOnHover(mousePos));
                selectedPieceInfo = -1;
                pieces.setIsPieceSelected(false);
                inputState = InputState::Normal;
                nextTurn();
            }
        }
        pieces.setSelectedPieceIndex(selectedPieceInfo);
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

void MyGame::nextTurn() {
    if (playerTeam == PieceTeam::White) playerTeam = PieceTeam::Black;
    else playerTeam = PieceTeam::White;
}