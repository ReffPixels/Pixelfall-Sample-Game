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
    playerToMove = PieceTeam::White;

    return true;
}

// Called every frame
void MyGame::onUpdate() {

    // Track FPS in window title
    appWindow->setWindowTitle("Chess | FPS: " + std::to_string(appClock->getFPS()));

    // Store Cursor Position
    cursorPos = appWindow->physicalToLogical(appInput->getMousePhysicalPosition());

    // Center Board
    board->setPosition((appWindow->getLogicalSize() / 2) - (board->getTileSize() * 4));

    // Handle board interaction
    if (board->isBoardOnHover(cursorPos)) {
        Vector2Int hoveredSquare = ChessPieces::getPosFromNotation(board->getSquareOnHover(cursorPos));
        if (appInput->isMouseButtonPressed(MouseButton::Left))  onBoardPressed(hoveredSquare);
        if (appInput->isMouseButtonReleased(MouseButton::Left)) onBoardReleased(hoveredSquare);
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
    board->draw(*painter);

    // Draw Selected Highlight
    highlightSelected();

    // Draw Last Move
    highlightLastMove();

    // Hide selected piece if in drag and drop mode (Must be done before the pieces are drawn to avoid flicker)
    pieces.setHideSelectedPiece(
        inputState == InputState::PieceSelected && appInput->isMouseButtonDown(MouseButton::Left));

    // Draw pieces
    pieces.drawPieces(
        boardState, board->getPosition(), board->getTileSize(), board->getTileSize(), *painter);

    // Drag and Drop Visuals
    if (inputState == InputState::PieceSelected && appInput->isMouseButtonDown(MouseButton::Left)) {
        pieceFollowCursor(
            boardState[selectedPiecePosition.x][selectedPiecePosition.y].type,
            boardState[selectedPiecePosition.x][selectedPiecePosition.y].team,
            dragAndDropPivot
        );
        // Hide selected piece (Avoid duplication)
        pieces.setHideSelectedPiece(true);
    }
    else {
        pieces.setHideSelectedPiece(false);
    }
}

// Pressing allows to select a new piece or move if a piece is already selected.
void MyGame::onBoardPressed(Vector2Int square) {
    PieceInfo& clicked = boardState[square.x][square.y];
    if (clicked.type != PieceType::None && clicked.team == playerToMove)
        selectPiece(square);
    else
        moveSelectedPiece(square);
}

// Release only allows to move if a piece is already selected. (Drag and drop behaviour) Otherwise it does nothing.
void MyGame::onBoardReleased(Vector2Int square) {
    moveSelectedPiece(square);
}

// Selects a piece in a specific square and changes the input state to selected.
void MyGame::selectPiece(Vector2Int selectedSquare) {
    selectedPiecePosition = selectedSquare;
    pieces.setSelectedPiecePosition(selectedSquare);
    inputState = InputState::PieceSelected;
    // Adjust drag and drop pivot point
    dragAndDropPivot = {cursorPos - (board->getPosition() +
        Vector2{(float)selectedPiecePosition.x, (float)selectedPiecePosition.y} * board->getTileSize())};
}

// Moves the selected piece to a new square and updates the board state to match. 
// A succesful move triggers the next turn.
void MyGame::moveSelectedPiece(Vector2Int targetSquare) {
    if (inputState != InputState::PieceSelected) return;
    if (targetSquare == selectedPiecePosition) return; // released on same square, just a click-to-select

    PieceInfo& selected = boardState[selectedPiecePosition.x][selectedPiecePosition.y];
    PieceInfo& target = boardState[targetSquare.x][targetSquare.y];

    // There's a piece in the target square!
    if (target.type != PieceType::None) 
        if (target.team == playerToMove) return; // It's an an ally piece (Invalid move)   
        else moveRuleCounter = 0; // It's a capture (Reset 50 move rule)
    else moveRuleCounter++;

    // Move Piece
    target = selected;
    selected = {PieceType::None, PieceTeam::None};

    // Record Move
    lastMoveTarget = targetSquare;
    lastMoveOrigin = selectedPiecePosition;

    // Castling Rights
    updateCastlingRights();

    // Reset Trackers
    selectedPiecePosition = {-1, -1};
    pieces.setSelectedPiecePosition({-1, -1});
    inputState = InputState::Normal;


    nextTurn();
}

// Swaps the current player (Local)
void MyGame::nextTurn() {
    if (playerToMove == PieceTeam::White) {
        playerToMove = PieceTeam::Black;
    }
    else if (playerToMove == PieceTeam::Black) {
        playerToMove = PieceTeam::White;
        totalFullMoves++;
    };

    std::cout << "-------------------------------" << std::endl;
    std::cout << "Player to Move: " << static_cast<int>(playerToMove) << std::endl;
    std::cout << "Half Moves: " << moveRuleCounter << std::endl;
    std::cout << "Full Moves: " << totalFullMoves << std::endl;
    std::cout << "Castling Rights KQkq: " << wKingSideCastling << wQueenSideCastling
        << bKingSideCastling << bQueenSideCastling << std::endl;
}

// Draw a highlight on the square of the selected piece
void MyGame::highlightSelected() {
    if (selectedPiecePosition != Vector2Int{-1, -1}) {
        painter->drawRectangle(
            board->getPosition()
            + Vector2(selectedPiecePosition.x * board->getTileSize().x,
                selectedPiecePosition.y * board->getTileSize().y),
            board->getTileSize(),
            Color::fromHexcode("#ffd94e92")
        );
    }
}

// Draw a highlight on the start and end squares of the last move
void MyGame::highlightLastMove() {
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
}

// Draw a highlight on the square that the cursor is hovering
void MyGame::highlightHoveredSquare() {
    // Snap to grid position
    Vector2Int gridPos = ChessPieces::getPosFromNotation(board->getSquareOnHover(cursorPos));
    Vector2 snappedPositionInBoard{
        std::clamp(board->getPosition().x + (float)gridPos.x * board->getTileSize().x,
            board->getPosition().x, board->getPosition().x + board->getTileSize().x * 7),
        std::clamp(board->getPosition().y + gridPos.y * board->getTileSize().y,
            board->getPosition().y, board->getPosition().y + board->getTileSize().y * 7)
    };

    // Draw highlight border
    painter->drawRectangleHollow(
        snappedPositionInBoard,
        board->getTileSize(),
        board->getTileSize() * 0.9f,
        Color::fromHexcode("#ffffffbd")
    );
}

// Attach a piece visual to the cursor
void MyGame::pieceFollowCursor(PieceType pieceType, PieceTeam pieceTeam, Vector2 offset) {
    // Clamp cursor position to inside the board
    Vector2 positionInBoard{
        std::clamp(cursorPos.x,
            board->getPosition().x, board->getPosition().x + board->getTileSize().x * 8),
        std::clamp(cursorPos.y,
            board->getPosition().y, board->getPosition().y + board->getTileSize().y * 8)
    };

    pieces.drawFree(
        pieceType, pieceTeam,
        positionInBoard - offset, board->getTileSize(), *painter);
}

// Update Castling Rights
void MyGame::updateCastlingRights() {
    if (playerToMove == PieceTeam::White) {
        // Moved King-side Rook
        if (wKingSideCastling && selectedPiecePosition == Vector2Int{7, 7}) {
            wKingSideCastling = false;
        }
        // Moved Queen-side Rook
        if (wQueenSideCastling && selectedPiecePosition == Vector2Int{0, 7}) {
            wQueenSideCastling = false;
        }
        // Moved the king
        if ((wKingSideCastling || wQueenSideCastling) && selectedPiecePosition == Vector2Int{4, 7}) {
            wKingSideCastling = false;
            wQueenSideCastling = false;
        }
    }
    else if (playerToMove == PieceTeam::Black) {
        // Moved King-side Rook
        if (bKingSideCastling && selectedPiecePosition == Vector2Int{7, 0}) {
            bKingSideCastling = false;
        }
        // Moved Queen-side Rook
        if (bQueenSideCastling && selectedPiecePosition == Vector2Int{0, 0}) {
            bQueenSideCastling = false;
        }
        // Moved the king
        if ((bKingSideCastling || bQueenSideCastling) && selectedPiecePosition == Vector2Int{4, 0}) {
            bKingSideCastling = false;
            bQueenSideCastling = false;
        }
    }
}

// [TODO] Lerp animation piece when clicking instead of dragging