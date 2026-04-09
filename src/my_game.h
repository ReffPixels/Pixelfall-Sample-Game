// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
// Engine
#include "pixelfall/engine/core/application.h"
// Chess
#include "chess_game.h"
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

    // Chess Game
    ChessGame game;
};