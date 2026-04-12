// Defines the state of a chess game. 
// The main purpose of GameState is to take a move as input, and update the state of the board to match.

#pragma once
// Chess
#include "config/chess_config.h"
#include "tools/fen_parser.h"
#include "game_objects/chess_board.h"
#include "game_objects/chess_pieces.h"
// Standard Library
#include <memory>
#include <array>
#include <vector>

// Records all of the information necessary to track the current state of the game.
struct BoardState {
    std::vector<Piece> pieces; // List of every active piece
    std::array<std::array<Tile, 8>, 8> tiles; // 8x8 2D array of files and ranks 
    TeamColor playerToMove{TeamColor::White}; // Player that gets to move next.
    CastlingRights castlingRights{true, true, true, true}; // Tracker for who's lost their castling rights.
    Vector2Int enPassantTargetSquare{-1, -1}; // Record where en passant is possible (Cannot be deduced from positions)
    int moveRuleCounter{0}; // Used for 50 fold and 75 fold repetition. Counts on every pawn move or capture.
    int totalFullMoves{1}; // Starts at 1 due to some arcaic reason. Counts up only on black moves.
};

class GameState {
public:
    // Setup
    void setupFromFEN(std::string fenString);

    // State Control
    const BoardState& getBoardState() const { return boardState; };
    void syncPieceState();
    void syncTileState();
    void clearState();
    void clearTileState();
    void updateCastlingRights();
    void incrementTotalMoves();

    // User Interaction (Input that modifies the game state)
    void movePiece(Vector2Int origin, Vector2Int target, MoveType moveType);
    void selectPromotionPiece(PieceType pieceType);
    void swapPlayers();

    // Getters
    Move getLastMove() const { return lastMove; };
    TeamColor getOpponent();

    // Debug
    void removePiece(Vector2Int position);

private:
    // Board State
    BoardState boardState;
    std::string currentBoardFEN; // [TODO] Description of the current board in FEN

    // Board Memory
    Move lastMove;
};