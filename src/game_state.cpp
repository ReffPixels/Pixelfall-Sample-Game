// Implementation for game_state.h

#include "game_state.h"
#include "move_generation.h"
// Standard Library
#include <iostream>
#include <algorithm>

// Builds the board state from a FEN string.
void GameState::setupFromFEN(std::string fenString) {
    // Get FEN of the initial position
    FenState fenState{fen_parser::getState(fenString)};

    // Assign FEN values to the board state
    boardState.pieces = fenState.pieceList;
    boardState.playerToMove = fenState.playerToMove;
    boardState.castlingRights = fenState.castlingRights;
    boardState.enPassantTargetSquare = fenState.enPassantTargetSquare;
    boardState.moveRuleCounter = fenState.moveRuleCounter;
    boardState.totalFullMoves = fenState.totalFullMoves;

    // We have to manually sync the tileState since FEN only provides us with the pieceState
    syncTileState();
}

// Updates the piece list to match the board state.
void GameState::syncPieceState() {

    boardState.pieces.clear();
    for (int r = 0; r < 8; r++)
        for (int f = 0; f < 8; f++)
            if (boardState.tiles[f][r].type != PieceType::None)
                boardState.pieces.push_back({
                    boardState.tiles[f][r].type,
                    boardState.tiles[f][r].team,
                    Vector2Int(f, r)}
                    );
}

// Updates the board state to match the piece list.
void GameState::syncTileState() {

    clearTileState();
    for (const auto& piece : boardState.pieces)
        boardState.tiles[piece.position.x][piece.position.y] = {piece.type, piece.team};
}

// Empties every tile in the tile state. 
// Unlike the pieces vector that can just use.clear(), this needs its own function.
void GameState::clearTileState() {

    for (auto& file : boardState.tiles)
        for (auto& tile : file)
            tile = {PieceType::None, TeamColor::None};
}

// Resets the board state, piece list and any other game state trackers.
void GameState::clearState() {

    clearTileState();
    boardState.pieces.clear();
    boardState.playerToMove = TeamColor::White;
    boardState.castlingRights = {true, true, true, true};
    boardState.enPassantTargetSquare = {-1, -1};
    boardState.moveRuleCounter = 0;
    boardState.totalFullMoves = 1;
    lastMove = {{-1,-1},{-1,-1}};
}

// Moves a piece from an origin square to a target square and applies rules based on the type of move.
void GameState::movePiece(Vector2Int origin, Vector2Int target, MoveType moveType) {

    Tile& selected = boardState.tiles[origin.x][origin.y];
    Tile& targetPiece = boardState.tiles[target.x][target.y];

    // Reset enPassant availability
    boardState.enPassantTargetSquare = {-1, -1};

    // Reset move rule counter if this was a capture or a pawn move
    if (targetPiece.type != PieceType::None || selected.type == PieceType::Pawn) boardState.moveRuleCounter = 0;
    else boardState.moveRuleCounter++;

    // Move piece
    targetPiece = selected;
    lastMove.origin = origin;
    lastMove.target = target;

    // Was it a double pawn push? Record en passant square.
    if (selected.type == PieceType::Pawn && abs(target.y - origin.y) == 2)
        boardState.enPassantTargetSquare = {target.x, (origin.y + target.y) / 2};

    // Was it en passant? Remove the captured pawn.
    if (moveType == MoveType::EnPassant)
        boardState.tiles[target.x][origin.y] = {PieceType::None, TeamColor::None};

    // Was it a promotion?
    if (moveType == MoveType::Promotion || moveType == MoveType::CapturePromotion) {
        // We don't actually need to do anything here, since promotion is handled by user input.
        // [TODO] Consider removing this.
    }

    // Was it castling? Move the rook to its new position
    if (selected.team == TeamColor::White) {
        if (moveType == MoveType::CastlingKingSide) {
            boardState.tiles[7][7] = {PieceType::None, TeamColor::None};
            boardState.tiles[5][7] = {PieceType::Rook, TeamColor::White};
        }
        else if (moveType == MoveType::CastlingQueenSide) {
            boardState.tiles[0][7] = {PieceType::None, TeamColor::None};
            boardState.tiles[3][7] = {PieceType::Rook, TeamColor::White};
        }
    }
    else if (selected.team == TeamColor::Black) {
        if (moveType == MoveType::CastlingKingSide) {
            boardState.tiles[7][0] = {PieceType::None, TeamColor::None};
            boardState.tiles[5][0] = {PieceType::Rook, TeamColor::Black};
        }
        else if (moveType == MoveType::CastlingQueenSide) {
            boardState.tiles[0][0] = {PieceType::None, TeamColor::None};
            boardState.tiles[3][0] = {PieceType::Rook, TeamColor::Black};
        }
    }

    // Remove piece from its previous position
    selected = {PieceType::None, TeamColor::None};
}

// Update Castling Rights (Must be called at the end of a move, since it uses lastMove.origin and lastMove.target)
void GameState::updateCastlingRights() {

    CastlingRights cr = boardState.castlingRights;
    // White King-side Rook
    if (cr.whiteKingSide &&
        ((lastMove.origin == Vector2Int{7, 7}) || (lastMove.target == Vector2Int{7, 7}))) {
        cr.whiteKingSide = false;
    }
    // White Queen-side Rook
    if (cr.whiteQueenSide &&
        ((lastMove.origin == Vector2Int{0, 7}) || (lastMove.target == Vector2Int{0, 7}))) {
        cr.whiteQueenSide = false;
    }
    // White King
    if ((cr.whiteKingSide || cr.whiteQueenSide) && lastMove.origin == Vector2Int{4, 7}) {
        cr.whiteKingSide = false;
        cr.whiteQueenSide = false;
    }
    // Black King-side Rook
    if (cr.blackKingSide &&
        ((lastMove.origin == Vector2Int{7, 0}) || (lastMove.target == Vector2Int{7, 0}))) {
        cr.blackKingSide = false;
    }
    // Black Queen-side Rook
    if (cr.blackQueenSide &&
        ((lastMove.origin == Vector2Int{0, 0}) || (lastMove.target == Vector2Int{0, 0}))) {
        cr.blackQueenSide = false;
    }
    // Black King
    if ((cr.blackKingSide || cr.blackQueenSide) && lastMove.origin == Vector2Int{4, 0}) {
        cr.blackKingSide = false;
        cr.blackQueenSide = false;
    }
    boardState.castlingRights = cr;
}

// Used to increment the total moves from game state instead of having to pass the board state.
void GameState::incrementTotalMoves() {

    if (boardState.playerToMove == TeamColor::Black) boardState.totalFullMoves++;
}

// Transforms the promoted pawn into the piece provided (Queen, Rook, Bishop or Knight)
void GameState::selectPromotionPiece(PieceType pieceType) {

    boardState.tiles[lastMove.target.x][lastMove.target.y] = {pieceType, boardState.playerToMove};
}

// Returns the team color of the opponent
TeamColor GameState::getOpponent() {

    if (boardState.playerToMove == TeamColor::White) return TeamColor::Black;
    if (boardState.playerToMove == TeamColor::Black) return TeamColor::White;
    return TeamColor::None;
}

// Swaps the player to move
void GameState::swapPlayers() {

    boardState.playerToMove = getOpponent();
}

// Used to remove a piece from the board state.
// This is just a debug feature, it is not used by game state to clear squares.
void GameState::removePiece(Vector2Int position) {

    boardState.tiles[position.x][position.y] = {PieceType::None, TeamColor::None};
}
