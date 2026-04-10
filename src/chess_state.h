// Defines behaviours of the chess game

#pragma once
// Chess
#include "config/board_config.h"
#include "tools/fen_parser.h"
#include "game_objects/chess_board.h"
#include "game_objects/chess_pieces.h"
// Standard Library
#include <memory>
#include <array>
#include <vector>

enum class GameOutcome { // [TODO] Victory detection
    Undecided,
    WhiteVictoryCheckmate,
    BlackVictoryCheckmate,
    WhiteVictoryResignation,
    BlackVictoryResignation,
    WhiteVictoryTimeout,
    BlackVictoryTimeout,
    DrawStalemate,
    DrawInsufficientMaterial,
    Draw50Move,
    Draw75Move,
    Draw3FoldRepetition,
    Draw5FoldRepetition,
    DrawAgreement,
};

enum class InputState { Normal, PieceSelected, };

enum class MoveType { None, Move, Capture, EnPassant, CastlingKingSide, CastlingQueenSide, Promotion, CapturePromotion };

struct Move { Vector2Int origin; Vector2Int target; };

struct CastlingRights {
    bool whiteKingSide;
    bool whiteQueenSide;
    bool blackKingSide;
    bool blackQueenSide;
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
    static std::array<std::array<bool, 8>, 8> getAttackedSquares(bool ignoreKing, PieceTeam playerTeam,
        const std::array<std::array<PieceInfo, 8>, 8>& boardState, const CastlingRights& castlingRights);
    void nextTurn();

    // Getters
    const std::array<std::array<PieceInfo, 8>, 8>& getBoardState() const { return boardState; }
    InputState getInputState() const { return inputState; }
    Vector2Int getselPiecePosition() const { return selPiecePosition; }
    Vector2Int getLastMoveOrigin() const { return lastMoveOrigin; }
    Vector2Int getLastMoveTarget() const { return lastMoveTarget; }
    const std::array<std::array<MoveType, 8>, 8>& getValidMoves() const { return validMoves; }
    PieceTeam getPlayerToMove() const { return playerToMove; }
    const CastlingRights& getCastlingRights() const { return castlingRights; }

private:
    FenParser fenParser;

    // State
    std::string currentBoardFEN = board::defaults::defaultBoardFEN.data(); // Description of the current board
    std::array<std::array<PieceInfo, 8>, 8> boardState; // 8x8 2D array of files and ranks 
    PieceTeam playerToMove{PieceTeam::White}; // White plays first
    CastlingRights castlingRights{ true, true, true, true }; // Tracker for who's lost their castling rights.
    Vector2Int enPassantTargetSquare{-1, -1}; // [TODO]
    int moveRuleCounter{0}; // Used for 50 fold and 75 fold repetition. Counts on every pawn move or capture.
    int totalFullMoves{1}; // Starts at 1 due to some arcaic reason. Counts up only on black moves.
    GameOutcome gameOutcome{GameOutcome::Undecided};

    // Last Move
    Vector2Int lastMoveOrigin{-1, -1};
    Vector2Int lastMoveTarget{-1, -1};

    // Captured Pieces
    std::vector<PieceInfo> wCapturedPieces; // White's graveyard (Captured white pieces)
    std::vector<PieceInfo> bCapturedPieces; // Black's graveyard (Captured black pieces)

    // Interaction
    InputState inputState{InputState::Normal};
    Vector2Int selPiecePosition{-1, -1};  // (-1, -1) means no square is selected.

    // Moves
    std::array<std::array<MoveType, 8>, 8> validMoves;
};

// [TODO] Some way to track 3 fold and 5 fold repetition (FEN snapshops?)
// [TODO] Detect Game Outcomes and reset game
// [TODO] Hover highlight should not hover origin (flash)
// [TODO] Lerp animation piece when clicking instead of dragging