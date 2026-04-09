// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
// Engine
#include "pixelfall/engine/core/application.h"
// Chess
#include "config/board_config.h"
#include "chess_pieces.h"
#include "chess_board.h"
#include "fen_parser.h"
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

enum class InputState {
    Normal,
    PieceSelected,
};

class MyGame : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;

    // Update Methods
    void onBoardPressed(Vector2Int square);
    void onBoardReleased(Vector2Int square);

    // Gameplay
    void selectPiece(Vector2Int selectedSquare);
    void moveSelectedPiece(Vector2Int targetSquare);
    void updateCastlingRights();
    void nextTurn();

    // Rendering
    void highlightSelected();
    void highlightLastMove();
    void highlightHoveredSquare();
    void pieceFollowCursor(PieceType pieceType, PieceTeam pieceTeam, Vector2 offset = Vector2::Zero);

private:
    // Input
    Vector2 cursorPos{};

    // Board creation
    std::string currentBoardFEN = board::defaults::defaultBoardFEN.data();
    FenParser fenParser;
    ChessPieces pieces;
    std::unique_ptr<ChessBoard> board;

    // Game State
    std::array<std::array<PieceInfo, 8>, 8> boardState; // 8x8 2D array of files and ranks 
    PieceTeam playerToMove{};
    bool wKingSideCastling{true};
    bool wQueenSideCastling{true};
    bool bKingSideCastling{true};
    bool bQueenSideCastling{true};
    Vector2Int enPassantTargetSquare{-1, -1}; // [TODO]
    int moveRuleCounter{0}; // Used for 50 fold and 75 fold repetition. Counts on every pawn move or capture.
    int totalFullMoves{1}; // Starts at 1 due to some arcaic reason. Counts up only on black moves.
    GameOutcome gameOutcome{GameOutcome::Undecided};

    // Complete history of this match. Necessary for three-fold and five-fold repetition checks
    std::string PGN; // [TODO]

    // Interaction
    Vector2Int selectedPiecePosition{-1, -1};  // (-1, -1) means no square is selected.
    InputState inputState{InputState::Normal};

    // Visualizations
    Vector2Int lastMoveOrigin{-1, -1}; // Used for highlighting
    Vector2Int lastMoveTarget{-1, -1}; // Used for highlighting
    std::vector<PieceInfo> wCapturedPieces; // White's graveyard (Captured white pieces)
    std::vector<PieceInfo> bCapturedPieces; // Black's graveyard (Captured black pieces)
    Vector2 dragAndDropPivot{Vector2::Zero}; // Used to attach the piece to the cursor
};