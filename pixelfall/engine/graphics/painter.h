// Allows drawing geometrical shapes by creating meshes.

#pragma once
// Graphics
#include "pixelfall/engine/graphics/shader.h"
#include "pixelfall/engine/graphics/color.h"
// Core
#include "pixelfall/engine/core/window.h"
// Math
#include "pixelfall/engine/math/vector2.h"

class Painter {
public:
    Painter(Shader& shader, Window& window)
        : shader(shader), window(window) {}

    // Methods
    void drawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color);

private:
    Shader& shader;
    Window& window;
};