// Useful mathematical tools
#pragma once
// Math
#include "pixelfall/engine/math/numbers.h"

namespace math{
    namespace conversions {

        // Angles
        inline float degreesToRadians(float degrees) {
            return degrees * (float)((2 * numbers::pi) / 360.0f);
        }
        inline float radiansToDegrees(float radians) {
            return radians * (float)(360.0f / (2 * numbers::pi));
        }
    }
}