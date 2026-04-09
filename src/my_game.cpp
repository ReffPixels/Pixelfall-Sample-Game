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
    boardState = fenParser.getBoardFromFEN(currentBoardFEN);

    // White plays first
    playerTeam = PieceTeam::White;

    return true;
}

// Called every frame
void MyGame::onUpdate() {
    mousePos = appWindow->physicalToLogical(appInput->getMousePhysicalPosition());

    // Center board
    board->setPosition(
        (appWindow->getLogicalSize() / 2) - (board->getTileSize() * 4)
    );

    appWindow->setWindowTitle("Chess | FPS: " + std::to_string(appClock->getFPS()));

    if (appInput->isMouseButtonPressed(MouseButton::Left)
        || appInput->isMouseButtonReleased(MouseButton::Left)) {
        // Console Debug
        std::cout << board->getSquareOnHover(mousePos) << std::endl;

        // Get square clicked
        Vector2Int clickedSquare = ChessPieces::getPosFromNotation(board->getSquareOnHover(mousePos));

        // Check if there's a piece in that square
        PieceInfo& clickedPiece = boardState[clickedSquare.x][clickedSquare.y];
        bool squareIsEmpty = (clickedPiece.type == PieceType::None);

        // The Square clicked on has a piece on it
        if (!squareIsEmpty) {
            // The piece is on our team and there is no piece selected
            if (inputState == Normal && clickedPiece.team == playerTeam) {
                // Select piece
                selectedPiecePosition = clickedSquare;
                pieces.setSelectedPiecePosition(selectedPiecePosition);
                inputState = InputState::PieceSelected;
            }
            // The piece is an enemy piece and a piece is already selected
            else if (inputState == PieceSelected && clickedPiece.team != playerTeam) {
                // Replace enemy piece with selected piece
                boardState[clickedSquare.x][clickedSquare.y] = boardState[selectedPiecePosition.x][selectedPiecePosition.y];
                // Remove selected piece
                boardState[selectedPiecePosition.x][selectedPiecePosition.y] = {PieceType::None, PieceTeam::None};
                selectedPiecePosition = {-1, -1};
                inputState = InputState::Normal;
                nextTurn();
            }
        }
        // The Square clicked is an empty Square
        else {
            if (inputState == PieceSelected) {
                // Replace empty square with selected piece
                boardState[clickedSquare.x][clickedSquare.y] = boardState[selectedPiecePosition.x][selectedPiecePosition.y];
                // Remove selected piece
                boardState[selectedPiecePosition.x][selectedPiecePosition.y] = {PieceType::None, PieceTeam::None};
                selectedPiecePosition = {-1, -1};
                inputState = InputState::Normal;
                nextTurn();
            }
        }
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
    pieces.drawPieces(
        boardState, board->getPosition(), board->getTileSize(), board->getTileSize(), *painter);

    // Draw selected piece following mouse
    if (inputState == InputState::PieceSelected) {

        if (appInput->isMouseButtonDown(MouseButton::Left)) {
            pieces.setHideSelectedPiece(true);
            pieces.drawFree(
                boardState[selectedPiecePosition.x][selectedPiecePosition.y].type,
                boardState[selectedPiecePosition.x][selectedPiecePosition.y].team,
                mousePos - (board->getTileSize() / 2), board->getTileSize(), *painter);
        } else {
            pieces.setHideSelectedPiece(false);
        }
    }
}

void MyGame::nextTurn() {
    if (playerTeam == PieceTeam::White) playerTeam = PieceTeam::Black;
    else playerTeam = PieceTeam::White;
}