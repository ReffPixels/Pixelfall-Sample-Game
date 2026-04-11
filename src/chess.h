// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
// Engine
#include "pixelfall/engine/core/application.h"
// Chess
#include "chess_state.h"
#include "game_objects/chess_board.h"
#include "game_objects/chess_pieces.h"
#include "game_objects/promotion_ui.h"
#include "chess_visuals.h"
// Standard Library
#include <memory>
#include <array>
#include <vector>
#include <random>

class Chess : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;

private:
    // Input
    Vector2 cursorPos{};
    Vector2 dragAndDropPivot{Vector2::Zero};

    // Generate reusable random sequence with Mersenne Twister Algorithm.
    // [TODO] This should be provided by the Engine.
    std::mt19937 rng{std::random_device{}()};

    // Chess
    ChessState state;
    ChessBoard board;
    ChessPieces pieces;
    ChessVisuals visuals;
    PromotionInterface promotionUI;

    void startNewGame();
};