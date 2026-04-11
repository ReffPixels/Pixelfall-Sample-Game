#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"
// Standard Library
#include <string>
#include <vector>

// Utility functions for parsing algebraic notations
namespace NotationParser {
    // Converts a piece location from grid notation (a1 to h8) into a vector.
    static Vector2Int getPosFromNotation(const std::string& gridPosition) {
        return Vector2Int{
            gridPosition[0] - 'a', // 'a' -> 0, 'h' -> 7
            '8' - gridPosition[1]  // '8' -> 0, '1' -> 7
        };
    }

    // Converts a piece vector location into grid notation (a1 to h8).
    static std::string getNotationFromPos(Vector2Int gridPosition) {
        return std::string() +
            char('a' + gridPosition.x) +
            char('8' - gridPosition.y);
    };
};