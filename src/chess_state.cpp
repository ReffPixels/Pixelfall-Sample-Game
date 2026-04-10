// Implementation for chess_state.h

#include "chess_state.h"
#include "chess_moves.h"
// Standard Library
#include <iostream>
#include <algorithm>

void ChessState::setupFromFEN() {
    boardState = fenParser.getBoardFromFEN(currentBoardFEN);
}

// Pressing allows to select a new piece or move if a piece is already selected.
void ChessState::onBoardPressed(Vector2Int square) {
    PieceInfo& clicked = boardState[square.x][square.y];
    // It's a piece in the player's team
    if (clicked.type != PieceType::None && clicked.team == playerToMove)
        // Clicked on the same square, deselect
        if (square == selectedPosition) {
            deselectPiece();
            return;
        } else selectPiece(square);
    else
        moveSelectedPiece(square);
}

// Release only allows to move if a piece is already selected. (Drag and drop behaviour) Otherwise it does nothing.
void ChessState::onBoardReleased(Vector2Int square) {
    moveSelectedPiece(square);
}

// Selects a piece in a specific square and changes the input state to selected.
void ChessState::selectPiece(Vector2Int selectedSquare) {
    selectedPosition = selectedSquare;
    inputState = InputState::PieceSelected;
    PieceTeam team = boardState[selectedPosition.x][selectedPosition.y].team;
    switch (boardState[selectedPosition.x][selectedPosition.y].type) {
    case PieceType::King:
        validMoves = ChessMoves::generateKingMoves(selectedPosition, team, boardState, castlingRights);
        break;
    case PieceType::Queen:
        validMoves = ChessMoves::generateQueenMoves(selectedPosition, team, boardState);
        break;
    case PieceType::Rook:
        validMoves = ChessMoves::generateRookMoves(selectedPosition, team, boardState);
        break;
    case PieceType::Bishop:
        validMoves = ChessMoves::generateBishopMoves(selectedPosition, team, boardState);
        break;
    case PieceType::Knight:
        validMoves = ChessMoves::generateKnightMoves(selectedPosition, team, boardState);
        break;
    case PieceType::Pawn: {
        bool isFirstMove{};
        if (team == PieceTeam::White) isFirstMove = (selectedPosition.y == 6);
        else isFirstMove = (selectedPosition.y == 1);

        validMoves = ChessMoves::generatePawnMoves(
            selectedPosition, team, boardState, enPassantTargetSquare, isFirstMove);
        break;
    }
    default: ChessMoves::clearMoves(validMoves);
    }
}

// Moves the selected piece to a new square and updates the board state to match. 
// A succesful move triggers the next turn.
void ChessState::moveSelectedPiece(Vector2Int targetSquare) {
    MoveType moveType = validMoves[targetSquare.x][targetSquare.y];

    if (inputState != InputState::PieceSelected) return;
    if (targetSquare == selectedPosition) return;
    if (moveType == MoveType::None) {
        deselectPiece();
        return;
    }

    movePiece(selectedPosition, targetSquare, moveType);
    deselectPiece();
    nextTurn();
}

// Moves a piece from an origin square to a target square and applies rules based on the type of move.
void ChessState::movePiece(Vector2Int origin, Vector2Int target, MoveType moveType) {
    PieceInfo& selected = boardState[origin.x][origin.y];
    PieceInfo& targetPiece = boardState[target.x][target.y];

    // Reset enPassant availability
    enPassantTargetSquare = {-1, -1};

    // Update move rule counter
    if (targetPiece.type != PieceType::None) moveRuleCounter = 0;
    else moveRuleCounter++;

    // Move piece
    targetPiece = selected;
    lastMoveOrigin = origin;
    lastMoveTarget = target;

    // Was it a double pawn push? Record en passant square.
    if (selected.type == PieceType::Pawn && abs(target.y - origin.y) == 2)
        enPassantTargetSquare = {target.x, (origin.y + target.y) / 2};

    // Was it en passant? Remove the captured pawn.
    if (moveType == MoveType::EnPassant)
        boardState[target.x][origin.y] = {PieceType::None, PieceTeam::None};

    // Was it castling? Move the rook to its new position
    if (selected.team == PieceTeam::White) {
        if (moveType == MoveType::CastlingKingSide) {
            boardState[7][7] = {PieceType::None, PieceTeam::None};
            boardState[5][7] = {PieceType::Rook, PieceTeam::White};
        }
        else if (moveType == MoveType::CastlingQueenSide) {
            boardState[0][7] = {PieceType::None, PieceTeam::None};
            boardState[3][7] = {PieceType::Rook, PieceTeam::White};
        }
    }
    else if (selected.team == PieceTeam::Black) {
        if (moveType == MoveType::CastlingKingSide) {
            boardState[7][0] = {PieceType::None, PieceTeam::None};
            boardState[5][0] = {PieceType::Rook, PieceTeam::Black};
        }
        else if (moveType == MoveType::CastlingQueenSide) {
            boardState[0][0] = {PieceType::None, PieceTeam::None};
            boardState[3][0] = {PieceType::Rook, PieceTeam::Black};
        }
    }

    // Remove piece from its previous position
    selected = {PieceType::None, PieceTeam::None};
}


// Swaps the current player (Local)
void ChessState::nextTurn() {
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
    std::cout << "Castling Rights KQkq: " << castlingRights.whiteKingSide << castlingRights.whiteQueenSide
        << castlingRights.blackKingSide << castlingRights.blackQueenSide << std::endl;
}

// Update Castling Rights (Must be called at the end of a move, since it uses lastMoveOrigin and lastMoveTarget)
void ChessState::updateCastlingRights() {
    // White King-side Rook
    if (castlingRights.whiteKingSide &&
        ((lastMoveOrigin == Vector2Int{7, 7}) || (lastMoveTarget == Vector2Int{7, 7}))) {
        castlingRights.whiteKingSide = false;
    }
    // White Queen-side Rook
    if (castlingRights.whiteQueenSide &&
        ((lastMoveOrigin == Vector2Int{0, 7}) || (lastMoveTarget == Vector2Int{0, 7}))) {
        castlingRights.whiteQueenSide = false;
    }
    // White King
    if ((castlingRights.whiteKingSide || castlingRights.whiteQueenSide) && lastMoveOrigin == Vector2Int{4, 7}) {
        castlingRights.whiteKingSide = false;
        castlingRights.whiteQueenSide = false;
    }
    // Black King-side Rook
    if (castlingRights.blackKingSide &&
        ((lastMoveOrigin == Vector2Int{7, 0}) || (lastMoveTarget == Vector2Int{7, 0}))) {
        castlingRights.blackKingSide = false;
    }
    // Black Queen-side Rook
    if (castlingRights.blackQueenSide &&
        ((lastMoveOrigin == Vector2Int{0, 0}) || (lastMoveTarget == Vector2Int{0, 0}))) {
        castlingRights.blackQueenSide = false;
    }
    // Black King
    if ((castlingRights.blackKingSide || castlingRights.blackQueenSide) && lastMoveOrigin == Vector2Int{4, 0}) {
        castlingRights.blackKingSide = false;
        castlingRights.blackQueenSide = false;
    }
}

// Reset selected piece trackers
void ChessState::deselectPiece() {
    selectedPosition = {-1, -1};
    inputState = InputState::Normal;
    ChessMoves::clearMoves(validMoves);
}