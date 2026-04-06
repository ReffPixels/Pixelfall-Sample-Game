// Defines a chess piece object and handles drawing it on the screen.

#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/painter.h"
#include "pixelfall/engine/graphics/texture_cache.h"

enum PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};

enum PieceTeam {
    White,
    Black
};

struct PieceInfo {
    PieceType type;
    PieceTeam team;
    Vector2Int position{0, 0};
};

class ChessPiece {
public:
    // Methods
    void draw(PieceInfo pieceInfo, Vector2 boardPosition, Vector2 tileSize,
        Painter& painter, TextureCache& textureCache, const std::filesystem::path& projectPath);
};