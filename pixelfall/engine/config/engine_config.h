// Defines basic information about the engine.

#pragma once
// Standard Library
#include <string_view>

namespace engine {

    // Metadata
    constexpr std::string_view engineTitle {"Pixelfall"};
    constexpr std::string_view engineIdentifier {"pixelfall"};
    constexpr std::string_view engineVersion {"0.0"};

    namespace defaults {
        constexpr bool limitFrameRate {false};
        constexpr double fpsLimit {120.0};
    }
}