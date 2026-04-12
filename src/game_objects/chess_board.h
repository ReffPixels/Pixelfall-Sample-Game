// Defines a chess board object and handles drawing it on the screen.

#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/painter.h"
// Config
#include "config/theme_config.h"

// The board direction is simply the perspective we are visualizing the board from. (Black or White's point of view)
// This does not determine who plays first.
enum class BoardDirection {
    BlackOnTop,
    WhiteOnTop
};

enum class TileColor {
    Light,
    Dark
};

class ChessBoard {
public:
    // Getters
    const Vector2& getPosition() const { return position; };
    const BoardDirection& getBoardDirection() const { return direction; };
    const Vector2& getTileSize() const { return tileSize; };

    // Setters
    void setPosition(Vector2 position) { this->position = position; };
    void setBoardDirection(BoardDirection direction) { this->direction = direction; };

    // Theme Setters
    void setTileSize(Vector2 tileSize) { this->tileSize = tileSize; };
    void setDarkTileColor(Color darkTileColor) { this->darkTileColor = darkTileColor; };
    void setLightTileColor(Color lightTileColor) { this->lightTileColor = lightTileColor; };

    // Methods
    void draw(Painter& painter);
    bool isBoardOnHover(Vector2 mousePosition);
    Vector2Int getTileOnHover(Vector2 mousePosition);
    int getRankByDirection(int rank);
    
private:
    // Display Settings
    Vector2 position{Vector2::Zero};
    BoardDirection direction{BoardDirection::BlackOnTop};

    // Theme
    Vector2 tileSize{theme::defaults::tileSize};
    Color lightTileColor{theme::defaults::lightTileColor};
    Color darkTileColor{theme::defaults::darkTileColor};
};