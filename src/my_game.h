// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
// Engine
#include "pixelfall/engine/core/application.h"
// Chess
#include "config/board_config.h"
#include "fen_parser.h"

class MyGame : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;

private:
    std::string currentBoardFEN = board::defaults::defaultBoardFEN.data();
    FenParser fenParser;

    // Test
    Vector2 pieceTestPosition{};
    ChessPiece pieceTest;
};