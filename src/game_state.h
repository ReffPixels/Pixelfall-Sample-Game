// Defines behaviours of the chess game

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
    void setupFromFEN();

    void movePiece(Vector2Int origin, Vector2Int target, MoveType moveType);
    void updateCastlingRights();
    void updatePieceList();
    bool hasInsufficientMaterial();

    // Getters
    const BoardState& getBoardState() const { return boardState; };
    Move getLastMove() const { return lastMove; };
    const std::array<std::array<MoveType, 8>, 8>& getValidMoves() const { return validMoves; };
    TeamColor getOpponent();
    const GameOutcome& getGameOutcome() const { return gameOutcome; };

    // Pawn Promotion
    Vector2Int getPromotionPosition() const { return promotionPosition; };
    void onPromotionSelected(PieceType pieceType);

    // Debug
    void removePiece(Vector2Int position);

private:
    // Current Board State
    std::string currentBoardFEN = defaults::startPositionFEN.data(); // Description of the current board in FEN
    BoardState boardState;
    Vector2Int promotionPosition{-1, -1};

    // Moves
    Move lastMove;
    std::array<std::array<MoveType, 8>, 8> validMoves;

    // Game Outcome
    GameOutcome gameOutcome{GameOutcome::Playing};
    bool isKingInCheck(TeamColor team) const;
    bool hasLegalMoves(TeamColor team) const;
    void findGameOutcome();
};