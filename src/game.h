// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
// Engine
#include "pixelfall/engine/core/application.h"
// Chess
#include "chess_state.h"
#include "chess_board.h"
#include "chess_pieces.h"
#include "chess_visuals.h"
// Standard Library
#include <memory>
#include <array>
#include <vector>

class MyGame : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;

private:
    // Input
    Vector2 cursorPos{};
    Vector2 dragAndDropPivot{Vector2::Zero};

    // Chess
    ChessState game;
    ChessBoard board;
    ChessPieces pieces;
    ChessVisuals visuals;
};