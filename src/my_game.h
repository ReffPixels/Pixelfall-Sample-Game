// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
#include "pixelfall/engine/core/application.h"
#include "chess_piece.h"

class MyGame : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;

private:
    Texture* pawnTexture = nullptr;
    ChessPiece* myPiece = nullptr;
};