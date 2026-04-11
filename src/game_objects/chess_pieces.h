// Defines a chess piece object and handles drawing it on the screen.

#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/painter.h"
// Standard Library
#include <array>
// Chess
#include "config/chess_config.h"

class ChessPieces {
public:
    // Rendering
    void draw(Piece piece, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
        Painter& painter, Vector2 pieceOffset = Vector2::Zero);

    void drawPieces(std::vector<Piece> pieces, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
        Painter& painter, Vector2 pieceOffset = Vector2::Zero, Vector2Int selPiecePosition = {-1, -1});

    void drawFree(Piece piece, Vector2 physicalPosition, Vector2 spriteSize,
        Painter& painter);

    // Settings
    void setHideSelectedPiece(bool hideSelectedPiece) { this->hideSelectedPiece = hideSelectedPiece; };
    void setFlippedPieces(bool flipPieces) { this->flipPieces = flipPieces; };

    // Notation Conversions
    static Vector2Int getPosFromNotation(std::string gridPosition);
    static std::string getNotationFromPos(Vector2Int gridPosition);

private:
    // Rendering
    bool hideSelectedPiece{false};
    bool flipPieces{false};
};