// Defines information about shaders.

#pragma once
// Standard Library
#include <string_view>
// Variable Types
#include "pixelfall/engine/math/vectors.h"
#include "pixelfall/engine/graphics/color.h"

namespace shader {

    namespace defaults {

        // [TO DO] This code explicitly references OpenGL. 
        // This should be separated with an ifdef in the future once that is defined in the CmakeLists
        constexpr std::string_view defaultVertexShader {"assets/shaders/opengl/vertex_shader.glsl"};
        constexpr std::string_view defaultFragmentShader {"assets/shaders/opengl/fragment_shader.glsl"};
    }
}