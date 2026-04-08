// Defines basic information about the project.

#pragma once
// Standard Library
#include <string_view>

namespace application {

    // Settings
    // [TODO] - Set up a better anti aliasing pipeline and choose from an enum. Figure out what's the deal with MSAA
    constexpr bool autoEnableFXAA(false);

    namespace defaults {
        // Metadata
        constexpr std::string_view appTitle {"Default Pixelfall Project"};
        constexpr std::string_view appIdentifier {"default_pixelfall_project"};
        constexpr std::string_view appVersion{"0.0"};
    }
}