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

        // Flat geometry shader (Used for geometry and primitives)
        constexpr std::string_view geometryVertShader{"assets/shaders/opengl/geometry.vert.glsl"};
        constexpr std::string_view geometryFragShader{"assets/shaders/opengl/geometry.frag.glsl"};

        // Texture/Sprite shader
        constexpr std::string_view spriteVertShader{"assets/shaders/opengl/sprite.vert.glsl"};
        constexpr std::string_view spriteFragShader{"assets/shaders/opengl/sprite.frag.glsl"};

        // Screen passes for Anti Aliasing
        constexpr std::string_view screenVertShader{"assets/shaders/opengl/screen.vert.glsl"};
        // MSAA
        constexpr std::string_view msaaFragShader{"assets/shaders/opengl/msaa_screen.frag.glsl"};
        // FXAA
        constexpr std::string_view fxaaFragShader{"assets/shaders/opengl/fxaa_screen.frag.glsl"};
    }
}