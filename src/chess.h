// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
// Engine
#include "pixelfall/engine/core/application.h"
// Chess
#include "game_state.h"
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

    // Game Stages
    void startNewGame();
    void nextTurn();
    void endGame();
    void resetGame();

    // Input
    Vector2 cursorPos{};
    Vector2 dragAndDropPivot{Vector2::Zero};
    void onBoardPressed(Vector2Int square);
    void onBoardReleased(Vector2Int square);

    // Interaction
    InputState inputState{InputState::Normal};
    Vector2Int selPiecePosition{-1, -1};  // (-1, -1) means no square is selected.
    Vector2Int promotionPosition{-1, -1};
    void selectPiece(Vector2Int selectedSquare);
    void deselectPiece();
    void moveSelectedPiece(Vector2Int targetSquare);

    // Generate reusable random sequence with Mersenne Twister Algorithm.
    // [TODO] This should be provided by the Engine.
    std::mt19937 rng{std::random_device{}()};

    // Debug
    void printStatistics();
};