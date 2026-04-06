// Defines information about shaders.
// [TO DO] This code explicitly references OpenGL shaders. Should be separated (Maybe with ifdef)

#pragma once
// Standard Library
#include <string_view>
// Variable Types
#include "pixelfall/engine/math/vectors.h"
#include "pixelfall/engine/graphics/color.h"

namespace shader {

    namespace defaults {

        // This should be separated with an ifdef in the future once that is defined in the CmakeLists
        constexpr std::string_view geometryVertShader {"assets/shaders/opengl/geometry.vert.glsl"};
        constexpr std::string_view geometryFragShader{"assets/shaders/opengl/geometry.frag.glsl"};

        // Screen passes for Anti Aliasing
        constexpr std::string_view screenVertShader{"assets/shaders/opengl/screen.vert.glsl"};
        // MSAA
        constexpr std::string_view msaaFragShader{"assets/shaders/opengl/msaa_screen.frag.glsl"};
        // FXAA
        constexpr std::string_view fxaaFragShader{"assets/shaders/opengl/fxaa_screen.frag.glsl"};
    }
}