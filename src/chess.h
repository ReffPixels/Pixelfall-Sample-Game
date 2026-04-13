// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
// Engine
#include "pixelfall/engine/core/application.h"
// Chess
#include "game_state.h"
#include "game_outcome.h"
#include "game_objects/chess_board.h"
#include "game_objects/chess_pieces.h"
#include "ui/promotion_widget.h"
#include "rendering/tile_highlights.h"
// Standard Library
#include <memory>
#include <array>
#include <vector>
#include <random>

enum class InputState {
    Normal,         // Default state. The user can select a piece.
    PieceSelected,  // The user has selected a piece. Moving and capturing is available.
    Promotion       // The user has moved a pawn to a promoting square and they need to choose a piece type.
};

class Chess : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;

private:
    // Chess Components
    GameState state;
    ChessBoard board;
    ChessPieces pieces;
    Outcome gameOutcome{Outcome::Playing};  // Game starts as "Playing"

    // Game Stages
    void startNewGame();
    void nextTurn();
    void resetGame();
    void displayOutcome();

    // Input
    Vector2 cursorPos{};
    Vector2 dragAndDropPivot{Vector2::Zero};
    void onBoardPressed(Vector2Int square);
    void onBoardReleased(Vector2Int square);

    // Interaction
    InputState inputState{InputState::Normal};
    Piece selectedPiece;
    std::array<std::array<MoveType, 8>, 8> selectedPieceMoves;
    void selectPiece(Vector2Int selectedSquare);
    void deselectPiece();
    void moveSelectedPiece(Vector2Int targetSquare, bool isDrag);

    // Rendering
    void drawPieces(bool dragAndDrop);

    // Piece Animation 
    // [TODO] Eventually the engine should have an animation system, but this is literally the only animation we need.
    struct PieceAnimation {
        Piece piece{};              // Piece at its target square
        Vector2Int origin{};          // origin square
        float progress{1.0f};       // 1.0 = complete or inactive
        static constexpr float duration = 0.1f;
        bool isActive() const { return progress < 1.0f; }
    };
    PieceAnimation pieceAnimation{};

    // Generate reusable random sequence with Mersenne Twister Algorithm.
    // [TODO] This should be provided by the Engine.
    std::mt19937 rng{std::random_device{}()};
};