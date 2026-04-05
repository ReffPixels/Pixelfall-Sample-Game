// Defines information about the geometry painter.

#pragma once
// Standard Library
#include <string_view>
// Variable Types
#include "pixelfall/engine/math/vectors.h"
#include "pixelfall/engine/graphics/color.h"

namespace painter {

    namespace defaults {

        // Default amount of segments in an arc when unspecified
        constexpr int arcSegments{16};

        // Determines how detailed circles are at different sizes. (radius / detail)
        // 1.0f means one segment for pixel in the radius
        // 0.5f means one segment for every two pixels in the radius
        constexpr float circleSegmentsDetail{0.5f};

        // Minimum amount of segments in a circle (Recommended at least 3, to avoid collapsing into a line)
        constexpr int minCircleSegments{3};

        // Maximum amount of segments in a circle (Recommended at least 64)
        constexpr int maxCircleSegments{100};
    }
}