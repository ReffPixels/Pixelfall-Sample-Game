// Defines information about the window.

#pragma once
// Standard Library
#include <string_view>
// Variable Types
#include "pixelfall/engine/math/vectors.h"
#include "pixelfall/engine/graphics/color.h"

namespace window {

    // Presentation Modes are explained in design.md
    enum class PresentationMode {
        Free,
        Letterbox,
        Crop,
        Stretch,
        Expand,
        ExpandHorizontal,
        ExpandVertical,
    };

    namespace defaults {

        // Default window properties
        constexpr std::string_view windowTitle{"Pixelfall"};
        inline const Vector2Int physicalSize{1280, 720};
        inline const Vector2Int referenceSize{1280, 720};
        inline const Vector2Int minWindowSize{320, 180};
        constexpr float minAspectRatio{1.0f / 1.0f};
        constexpr float maxAspectRatio{32.0f / 9.0f};
        constexpr float dprScale = 1.0f;
        constexpr PresentationMode presentationMode = PresentationMode::Letterbox;

        // The color used to clear the window. It determines the color of the bars in Letterbox presentation
        inline const Color clearColor{Color::fromHexcode("#000000")};
    }
}