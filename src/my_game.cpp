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
    // Track FPS in window title
    appWindow->setWindowTitle("Chess | FPS: " + std::to_string(appClock->getFPS()));
    // Get Mouse Position
    mousePos = appWindow->physicalToLogical(appInput->getMousePhysicalPosition());
    // Center Board
    board->setPosition((appWindow->getLogicalSize() / 2) - (board->getTileSize() * 4));
    // Handle board interaction
    if (board->isBoardOnHover(mousePos)) {
        Vector2Int hoveredSquare = ChessPieces::getPosFromNotation(board->getSquareOnHover(mousePos));
        if (appInput->isMouseButtonPressed(MouseButton::Left))  onBoardPressed(hoveredSquare);
        if (appInput->isMouseButtonReleased(MouseButton::Left)) onBoardReleased(hoveredSquare);
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

    // Draw Selected Highlight
    if (selectedPiecePosition != Vector2Int{-1, -1}) {
        painter->drawRectangle(
            board->getPosition()
            + Vector2(selectedPiecePosition.x * board->getTileSize().x,
                selectedPiecePosition.y * board->getTileSize().y),
            board->getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
    }

    // Draw Last Move
    if (lastMoveOrigin != Vector2Int{-1, -1} && lastMoveTarget != Vector2Int{-1, -1}) {
        painter->drawRectangle(
            board->getPosition()
            + Vector2(lastMoveOrigin.x * board->getTileSize().x,
                lastMoveOrigin.y * board->getTileSize().y),
            board->getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
        painter->drawRectangle(
            board->getPosition()
            + Vector2(lastMoveTarget.x * board->getTileSize().x,
                lastMoveTarget.y * board->getTileSize().y),
            board->getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
    }

    // Draw pieces
    pieces.drawPieces(
        boardState, board->getPosition(), board->getTileSize(), board->getTileSize(), *painter);

    // Draw selected piece following mouse
    if (inputState == InputState::PieceSelected) {
        // Only execute while holding the button down (Drag and Drop behaviour)
        if (appInput->isMouseButtonDown(MouseButton::Left)) {
            // Clamp mouse position to inside the board
            Vector2 positionInBoard{
                std::clamp(mousePos.x,
                    board->getPosition().x, board->getPosition().x + board->getTileSize().x * 8),
                std::clamp(mousePos.y,
                    board->getPosition().y, board->getPosition().y + board->getTileSize().y * 8)
            };

            // Snap to grid position
            Vector2Int gridPos = ChessPieces::getPosFromNotation(board->getSquareOnHover(mousePos));
            Vector2 snappedPositionInBoard{
                std::clamp(board->getPosition().x + (float)gridPos.x * board->getTileSize().x,
                    board->getPosition().x, board->getPosition().x + board->getTileSize().x * 7),
                std::clamp(board->getPosition().y + gridPos.y * board->getTileSize().y,
                    board->getPosition().y, board->getPosition().y + board->getTileSize().y * 7)
            };

            // Drag and drop highlight
            painter->drawRectangleHollow(
                snappedPositionInBoard,
                board->getTileSize(),
                board->getTileSize() * 0.9f,
                Color::fromHexcode("#ffffffbd")
            );

            pieces.setHideSelectedPiece(true);
            pieces.drawFree(
                boardState[selectedPiecePosition.x][selectedPiecePosition.y].type,
                boardState[selectedPiecePosition.x][selectedPiecePosition.y].team,
                positionInBoard - (board->getTileSize() / 2), board->getTileSize(), *painter);
        }
        else {
            pieces.setHideSelectedPiece(false);
        }
    }
}

// Pressing allows to select a new piece or move if a piece is already selected.
void MyGame::onBoardPressed(Vector2Int square) {
    PieceInfo& clicked = boardState[square.x][square.y];
    if (clicked.type != PieceType::None && clicked.team == playerTeam)
        selectPiece(square);
    else
        moveSelectedPiece(square);
}

// Release only allows to move if a piece is already selected. (Drag and drop behaviour) Otherwise it does nothing.
void MyGame::onBoardReleased(Vector2Int square) {
    moveSelectedPiece(square);
}

// Selects a piece in a specific square and changes the input state to selected.
void MyGame::selectPiece(Vector2Int square) {
    selectedPiecePosition = square;
    pieces.setSelectedPiecePosition(square);
    inputState = InputState::PieceSelected;
}

// Moves the selected piece to a new square and updates the board state to match. 
// A succesful move triggers the next turn.
void MyGame::moveSelectedPiece(Vector2Int square) {
    if (inputState != InputState::PieceSelected) return;
    if (square == selectedPiecePosition) return; // released on same square, just a click-to-select

    PieceInfo& target = boardState[square.x][square.y];
    if (target.type != PieceType::None && target.team == playerTeam) return; // own piece, blocked

    // Move Piece
    boardState[square.x][square.y] = boardState[selectedPiecePosition.x][selectedPiecePosition.y];
    boardState[selectedPiecePosition.x][selectedPiecePosition.y] = {PieceType::None, PieceTeam::None};

    // Record Move
    lastMoveTarget = square;
    lastMoveOrigin = selectedPiecePosition;

    // Reset Trackers
    selectedPiecePosition = {-1, -1};
    pieces.setSelectedPiecePosition({-1, -1});
    inputState = InputState::Normal;

    nextTurn();
}

// Swaps the current player (Local)
void MyGame::nextTurn() {
    if (playerTeam == PieceTeam::White) playerTeam = PieceTeam::Black;
    else playerTeam = PieceTeam::White;
}

// [TODO] Lerp animation piece when clicking instead of dragging