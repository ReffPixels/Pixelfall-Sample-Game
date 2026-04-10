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

enum class GameOutcome {
    Playing,                            // The game is currently in progress
    WhiteVictoryCheckmate,              // There are no legal moves for black and the king is under check
    BlackVictoryCheckmate,              // There are no legal moves for white and the king is under check
    WhiteVictoryResignation,            // [TODO] Button system
    BlackVictoryResignation,            // [TODO] Button system
    WhiteVictoryTimeout,                // [TODO] Clock
    BlackVictoryTimeout,                // [TODO] Clock
    DrawTimeoutVsInsufficientMaterial,  // [TODO] Clock
    DrawStalemate,                      // There are no legal moves for the player but their king is not under check
    DrawInsufficientMaterial,           // [TODO] Insufficient Material Detection
    Draw50Move,                         // [TODO] Button system (50 Move is not automatic)
    Draw75Move,                         // 75 Half moves have passed since the last capture or pawn move.
    Draw3FoldRepetition,                // [TODO] Fen snapshots?
    Draw5FoldRepetition,                // [TODO] Fen snapshots?
    DrawAgreement,                      // [TODO] Button system
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
    void endGame();
    void resetGame(PieceTeam startingPlayer = PieceTeam::White);

    // Getters
    const std::array<std::array<PieceInfo, 8>, 8>& getBoardState() const { return boardState; }
    InputState getInputState() const { return inputState; }
    Vector2Int getselPiecePosition() const { return selPiecePosition; }
    Vector2Int getLastMoveOrigin() const { return lastMoveOrigin; }
    Vector2Int getLastMoveTarget() const { return lastMoveTarget; }
    const std::array<std::array<MoveType, 8>, 8>& getValidMoves() const { return validMoves; }
    PieceTeam getPlayerToMove() const { return playerToMove; }
    PieceTeam getOpponent();
    const CastlingRights& getCastlingRights() const { return castlingRights; }
    const GameOutcome& getGameOutcome() const { return gameOutcome; };

private:
    FenParser fenParser;

    // State (FEN)
    std::string currentBoardFEN = board::defaults::defaultBoardFEN.data(); // Description of the current board
    std::array<std::array<PieceInfo, 8>, 8> boardState; // 8x8 2D array of files and ranks 
    PieceTeam playerToMove{PieceTeam::White}; // White plays first
    CastlingRights castlingRights{ true, true, true, true }; // Tracker for who's lost their castling rights.
    Vector2Int enPassantTargetSquare{-1, -1}; // [TODO]
    int moveRuleCounter{0}; // Used for 50 fold and 75 fold repetition. Counts on every pawn move or capture.
    int totalFullMoves{1}; // Starts at 1 due to some arcaic reason. Counts up only on black moves.

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

    // Game Outcome
    GameOutcome gameOutcome{GameOutcome::Playing};
    bool isKingInCheck(PieceTeam team) const;
    bool hasLegalMoves(PieceTeam team) const;
    void findGameOutcome();
};

// [TODO] Lerp animation piece when clicking instead of dragging

// Insufficient Material Rules:
// king versus king
// king and bishop versus king
// king and knight versus king
// king and bishop versus king and bishop with the bishops on the same color.