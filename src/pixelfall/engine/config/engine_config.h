// Defines basic information about the project.

#pragma once
#include <string_view>

namespace engine {

    // Metadata
    constexpr std::string_view appTitle = "Pixelfall";
    constexpr std::string_view appIdentifier = "pixelfall";
    constexpr std::string_view appVersion = "0.0";

    namespace defaults {
        constexpr bool limitFrameRate{false};
        constexpr double fpsLimit = 120.0;
    }
}