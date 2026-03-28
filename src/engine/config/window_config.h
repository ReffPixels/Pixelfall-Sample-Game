// Defines information about the window.

#pragma once
#include <string_view>
#include "engine/math/vector2.h"

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

    namespace default {

        // Default window values
        constexpr std::string_view windowTitle{"Pixelfall"};

        inline const Vector2Int physicalSize{1280, 720};
        inline const Vector2Int referenceSize{1280, 720};
        inline const Vector2Int minWindowSize{320, 180};
        constexpr float minAspectRatio{1.0f / 1.0f};
        constexpr float maxAspectRatio{32.0f / 9.0f};
        constexpr float dprScale = 1.0f;
        constexpr PresentationMode presentationMode = PresentationMode::Free;
    }
}