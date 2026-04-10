// Defines a chess piece object and handles drawing it on the screen.

#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/painter.h"
// Standard Library
#include <array>

enum class PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
    None
};

enum class PieceTeam {
    White,
    Black,
    None
};

struct PieceInfo {
    PieceType type;
    PieceTeam team;
};

class ChessPieces {
public:
    // Methods
    void draw(PieceInfo pieceInfo, Vector2Int piecePosition, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
        Painter& painter, Vector2 pieceOffset = Vector2::Zero);

    void drawPieces(std::array<std::array<PieceInfo, 8>, 8> pieces, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
        Painter& painter, Vector2 pieceOffset = Vector2::Zero, Vector2Int selPiecePosition = {-1, -1});

    void drawFree(PieceType type, PieceTeam team, Vector2 physicalPosition, Vector2 spriteSize,
        Painter& painter);

    // Setters
    void setHideSelectedPiece(bool hideSelectedPiece) { this->hideSelectedPiece = hideSelectedPiece; };

    // Getters
    static Vector2Int getPosFromNotation(std::string gridPosition);
    static std::string getNotationFromPos(Vector2Int gridPosition);

private:
    bool hideSelectedPiece{false};
};