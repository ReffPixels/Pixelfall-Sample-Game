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

    // Window Settings
    appWindow->setWReferenceSize({720, 720});

    // Start new chess game
    startNewGame();

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
    if (state.getGameOutcome() == GameOutcome::Playing) {
        if (board.isBoardOnHover(cursorPos)) {
            Vector2Int hoveredSquare = ChessPieces::getPosFromNotation(board.getSquareOnHover(cursorPos));
            if (appInput->isMouseButtonPressed(MouseButton::Left)) {
                state.onBoardPressed(hoveredSquare);
                // Capture drag pivot at the moment of selection
                if (state.getInputState() == InputState::PieceSelected)
                    dragAndDropPivot = visuals.computeDragPivot(cursorPos, board, state.getselPiecePosition());
            }
            if (appInput->isMouseButtonReleased(MouseButton::Left)) state.onBoardReleased(hoveredSquare);
        }
    }
    else {
        // Game has ended, only action is to reset the game
        if (appInput->isMouseButtonPressed(MouseButton::Left)) {
            startNewGame();
        }
    }
}

// Called at the end of each frame (Handles rendering)
void Chess::onRender() {
    // Draw Background
    Color backgroundColor{
        state.getGameOutcome() == GameOutcome::Playing ?
        Color::fromHexcode("#212121") : Color::fromHexcode("#33ff00")};
    
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getLogicalSize().x, appWindow->getLogicalSize().y),
        backgroundColor
    );

    // Draw Board
    board.draw(*painter);

    // Draw Attacked Squares (Player)
    if (appInput->isKeyDown(KeyCode::Q)) visuals.highlightAttackedSquares(ChessState::getAttackedSquares(
        false, state.getPlayerToMove(), state.getBoardState(), state.getCastlingRights()), board, *painter);

    // Draw Attacked Squares (Opponent)
    if (appInput->isKeyDown(KeyCode::E)) visuals.highlightAttackedSquares(ChessState::getAttackedSquares(
        false, state.getOpponent(), state.getBoardState(), state.getCastlingRights()), board, *painter,
        Color::fromHexcode("#0000ff88"));

    // Draw Highlights
    Vector2Int selPos = state.getselPiecePosition();
    Vector2Int lastOrigin = state.getLastMoveOrigin();
    Vector2Int lastTarget = state.getLastMoveTarget();
    visuals.highlightLastMove(board, lastOrigin, lastTarget, *painter);

    if (state.getInputState() == InputState::PieceSelected) {
        visuals.highlightSelected(board, selPos, *painter);
        visuals.highlightValidMoves(state.getValidMoves(), board, *painter);
    }

    bool dragAndDrop = state.getInputState() == InputState::PieceSelected
        && appInput->isMouseButtonDown(MouseButton::Left);
    // Draw Drag and Drop Highlight (Under pieces)
    if (dragAndDrop) visuals.highlightHoveredSquare(cursorPos, board, *painter, selPos);

    // Draw Pieces (hide selected piece during drag to avoid duplication)
    pieces.setHideSelectedPiece(dragAndDrop);
    pieces.drawPieces(state.getBoardState(), board.getPosition(), board.getTileSize(),
        board.getTileSize(), *painter, Vector2::Zero, selPos);

    // Drag and Drop Visuals (piece following cursor, over pieces)
    if (dragAndDrop) {
        visuals.pieceFollowCursor(cursorPos, pieces, board,
            state.getBoardState()[selPos.x][selPos.y], *painter, dragAndDropPivot);
    }
}

void Chess::startNewGame() {
    // Randomly choose who plays first using Bernoulli Distribution 
     // This returns a bool (True or False) at a given chance (50 % in this case) by using our random sequence (rng)
    std::bernoulli_distribution coinFlip(0.5);
    PieceTeam firstPlayer = (coinFlip(rng)) ? PieceTeam::White : PieceTeam::Black;

    // Set initial state of board.
    state.resetGame(firstPlayer);

    // We need to flip the board and pieces here since state does not handle board visuals.
    if (firstPlayer == PieceTeam::White) {
        board.setBoardDirection(BoardDirection::BlackOnTop);
        pieces.setFlippedPieces(false);
    }
    else {
        board.setBoardDirection(BoardDirection::WhiteOnTop);
        pieces.setFlippedPieces(true);
    }
}
