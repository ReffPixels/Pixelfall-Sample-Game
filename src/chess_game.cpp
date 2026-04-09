// Implementation for chess_game.h

#include "chess_game.h"
// Standard Library
#include <iostream>
#include <algorithm>

void ChessGame::setupFromFEN() {
    boardState = fenParser.getBoardFromFEN(currentBoardFEN);
}

// Pressing allows to select a new piece or move if a piece is already selected.
void ChessGame::onBoardPressed(Vector2Int square) {
    PieceInfo& clicked = boardState[square.x][square.y];
    // It's a piece in the player's team
    if (clicked.type != PieceType::None && clicked.team == playerToMove)
        // Clicked on the same square, deselect
        if (square == selectedPiecePosition) {
            selectedPiecePosition = {-1, -1};
            inputState = InputState::Normal;
            return;
        } else selectPiece(square);
    else
        moveSelectedPiece(square);
}

// Release only allows to move if a piece is already selected. (Drag and drop behaviour) Otherwise it does nothing.
void ChessGame::onBoardReleased(Vector2Int square) {
    moveSelectedPiece(square);
}

// Selects a piece in a specific square and changes the input state to selected.
void ChessGame::selectPiece(Vector2Int selectedSquare) {
    selectedPiecePosition = selectedSquare;
    inputState = InputState::PieceSelected;
    // Adjust drag and drop pivot point
    visuals.updateDragAndDropPoint();
}

// Moves the selected piece to a new square and updates the board state to match. 
// A succesful move triggers the next turn.
void ChessGame::moveSelectedPiece(Vector2Int targetSquare) {
    // A piece is not selected, don't move anything
    if (inputState != InputState::PieceSelected) return;
    // Clicked on the same square, don't move anything.
    if (targetSquare == selectedPiecePosition) return;

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
    lastMoveOrigin = selectedPiecePosition;
    lastMoveTarget = targetSquare;

    // Deselect Piece
    selectedPiecePosition = {-1, -1};
    inputState = InputState::Normal;

    nextTurn();
}

// Swaps the current player (Local)
void ChessGame::nextTurn() {
    // Castling Rights
    updateCastlingRights();

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

// Update Castling Rights (Must be called at the end of a move, since it uses lastMoveOrigin and lastMoveTarget)
void ChessGame::updateCastlingRights() {
    // White King-side Rook
    if (wKingSideCastling &&
        ((lastMoveOrigin == Vector2Int{7, 7}) || (lastMoveTarget == Vector2Int{7, 7}))) {
        wKingSideCastling = false;
    }
    // White Queen-side Rook
    if (wQueenSideCastling &&
        ((lastMoveOrigin == Vector2Int{0, 7}) || (lastMoveTarget == Vector2Int{0, 7}))) {
        wQueenSideCastling = false;
    }
    // White King
    if ((wKingSideCastling || wQueenSideCastling) && lastMoveOrigin == Vector2Int{4, 7}) {
        wKingSideCastling = false;
        wQueenSideCastling = false;
    }
    // Black King-side Rook
    if (bKingSideCastling &&
        ((lastMoveOrigin == Vector2Int{7, 0}) || (lastMoveTarget == Vector2Int{7, 0}))) {
        bKingSideCastling = false;
    }
    // Black Queen-side Rook
    if (bQueenSideCastling &&
        ((lastMoveOrigin == Vector2Int{0, 0}) || (lastMoveTarget == Vector2Int{0, 0}))) {
        bQueenSideCastling = false;
    }
    // Black King
    if ((bKingSideCastling || bQueenSideCastling) && lastMoveOrigin == Vector2Int{4, 0}) {
        bKingSideCastling = false;
        bQueenSideCastling = false;
    }
}