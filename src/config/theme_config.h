// Defines information about the chess theme.

#pragma once
// Standard Library
#include <string_view>
// Variable Types
#include "pixelfall/engine/math/vectors.h"
#include "pixelfall/engine/graphics/color.h"

// [TODO] Currently there is no way to change theme
namespace theme {
    
    namespace defaults {
        // Default Board
        inline const Vector2 tileSize{70.0f, 70.0f};
        inline const Color lightTileColor{Color::fromHexcode("#eaead2")};
        inline const Color darkTileColor{Color::fromHexcode("#4b7299")};

        // Default Chess Pieces
        inline const Vector2 spriteSize{70.0f, 70.0f};
        inline const std::filesystem::path spritesPath{"assets/image/themes/Cburnett/"};
        inline const Vector2 pieceOffset{Vector2::Zero};
    }
}