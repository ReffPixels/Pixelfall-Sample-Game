// Defines behaviours of the chess game

#pragma once
// Chess
#include "tools/fen_parser.h"
#include "game_objects/chess_board.h"
#include "game_objects/chess_pieces.h"
// Standard Library
#include <memory>
#include <array>
#include <vector>

enum class InputState {
    Normal,         // Default state. The user can select a piece.
    PieceSelected,  // The user has selected a piece. Moving and capturing is available.
    Promotion       // The user has moved a pawn to a promoting square and they need to choose a piece type.
};

struct BoardState {
    // State (FEN)
    std::vector<Piece> pieceList;
    TeamColor playerToMove{TeamColor::White}; // Player that gets to move next.
    CastlingRights castlingRights{true, true, true, true}; // Tracker for who's lost their castling rights.
    Vector2Int enPassantTargetSquare{-1, -1}; // Record where en passant is possible (Cannot be deduced from positions)
    int moveRuleCounter{0}; // Used for 50 fold and 75 fold repetition. Counts on every pawn move or capture.
    int totalFullMoves{1}; // Starts at 1 due to some arcaic reason. Counts up only on black moves.
};

class ChessState {
public:
    // Setup
    void setupFromFEN();

    // Update Methods
    void onBoardPressed(Vector2Int square);
    void onBoardReleased(Vector2Int square);

    // Gameplay
    void selectPiece(Vector2Int selectedSquare);
    void deselectPiece();
    void moveSelectedPiece(Vector2Int targetSquare);
    void movePiece(Vector2Int origin, Vector2Int target, MoveType moveType);
    void updateCastlingRights();
    static std::array<std::array<bool, 8>, 8> getAttackedSquares(bool ignoreKing, TeamColor playerTeam,
        const std::array<std::array<PieceInfo, 8>, 8>& boardState, const CastlingRights& castlingRights);
    void nextTurn();
    void endGame();
    void resetGame();
    void updatePieceList();
    bool hasInsufficientMaterial();

    // Debug
    void removePiece(Vector2Int position);
    void printStatistics();

    // Getters
    const BoardState& getBoardState() const { return boardStatus; }
    InputState getInputState() const { return inputState; }
    Vector2Int getselPiecePosition() const { return selPiecePosition; }
    Vector2Int getLastMoveOrigin() const { return lastMoveOrigin; }
    Vector2Int getLastMoveTarget() const { return lastMoveTarget; }
    const std::array<std::array<MoveType, 8>, 8>& getValidMoves() const { return validMoves; }
    TeamColor getOpponent();
    const GameOutcome& getGameOutcome() const { return gameOutcome; };
    Vector2Int getPromotionPosition() const { return promotionPosition; }
    void onPromotionSelected(PieceType pieceType);

private:
    FenParser fenParser;

    // Current Board State
    std::string currentBoardFEN = board::defaults::defaultBoardFEN.data(); // Description of the current board in FEN
    BoardState boardStatus;

    // Last Move
    // The current Board State does not record how it got there, so we need to track it separately.
    Vector2Int lastMoveOrigin{-1, -1};
    Vector2Int lastMoveTarget{-1, -1};

    // Interaction
    InputState inputState{InputState::Normal};
    Vector2Int selPiecePosition{-1, -1};  // (-1, -1) means no square is selected.
    Vector2Int promotionPosition{-1, -1};

    // Moves
    std::array<std::array<MoveType, 8>, 8> validMoves;

    // Game Outcome
    GameOutcome gameOutcome{GameOutcome::Playing};
    bool isKingInCheck(TeamColor team) const;
    bool hasLegalMoves(TeamColor team) const;
    void findGameOutcome();
};