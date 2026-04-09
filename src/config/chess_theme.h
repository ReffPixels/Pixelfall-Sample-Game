// Defines the contents of a chess theme
// TO DO

#pragma once
#include <string>
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/color.h"

enum ThemeID {
    Cburnett,
    DaniMaccari,
    DaniMaccariPerspective,
    ScreamingBrainStudios,
    ScreamingBrainStudiosIsometric
};

enum ThemeProjection {
    Rectangular,
    Isometric
};

enum ThemeBoardType {
    Quad,
    Tiled,      
    BoardSprite
};

enum SpriteFiltering {
    NearestNeighbour,
    Linear
};

struct Theme {
    // Metadata
    ThemeID id;
    std::string name;
    std::string path;
    // Board
    ThemeProjection projection{Rectangular};
    // ThemeBoardType boardType{Tiled};
    // float boardHeightDisplacement{0.0f};
    // float boardImageSize{};
    // Tiles
    // Vector2 tileImageSize{64.0f, 64.0f};
    Vector2 tileSize{64.0f, 64.0f};
    Color lightTileTint{Color::fromHexcode("#fff")};
    Color darkTileTint{Color::fromHexcode("#fff")};
    // Pieces
    float pieceHeightDisplacement{0.0f};
    Vector2 pieceImageSize{64.0f, 64.0f};
    // Display Settings
    SpriteFiltering spriteFiltering{Linear};
};

inline const Theme& getTheme(ThemeID id) {
    static const Theme themes[5] = {
        {
            // Metadata
            Cburnett, "Cburnett", "assets/image/themes/Cburnett/",
            // Board
            Rectangular,
            // Tiles
            {64.0f, 64.0f}, Color::fromHexcode("#f0d9b5"), Color::fromHexcode("#b58863"),
            // Pieces
            0.0f, {64.0f, 64.0f},
            // Display Settings
            Linear
        },
        {
            // Metadata
            DaniMaccari, "Cburnett", "assets/image/themes/DaniMaccari/Flat",
            // Board
            Rectangular,
            // Tiles
            {64.0f, 64.0f}, Color::fromHexcode("#f0d9b5"), Color::fromHexcode("#b58863"),
            // Pieces
            0.0f, {64.0f, 64.0f},
            // Display Settings
            Linear
        },
        {
            // Metadata
            DaniMaccariPerspective, "Cburnett", "assets/image/themes/DaniMaccari/Perspective",
            // Board
            Rectangular,
            // Tiles
            {64.0f, 64.0f}, Color::fromHexcode("#f0d9b5"), Color::fromHexcode("#b58863"),
            // Pieces
            0.0f, {64.0f, 64.0f},
            // Display Settings
            Linear
        },
        {
            // Metadata
            ScreamingBrainStudios, "Cburnett", "assets/image/themes/cburnett/",
            // Board
            Rectangular,
            // Tiles
            {64.0f, 64.0f}, Color::fromHexcode("#f0d9b5"), Color::fromHexcode("#b58863"),
            // Pieces
            0.0f, {64.0f, 64.0f},
            // Display Settings
            Linear
        },
        {
            // Metadata
            ScreamingBrainStudiosIsometric, "Cburnett", "assets/image/themes/cburnett/",
            // Board
            Isometric,
            // Tiles
            {64.0f, 64.0f}, Color::fromHexcode("#f0d9b5"), Color::fromHexcode("#b58863"),
            // Pieces
            0.0f, {64.0f, 64.0f},
            // Display Settings
            Linear
        },
    };
    return themes[id];
}