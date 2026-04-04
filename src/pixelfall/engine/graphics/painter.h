// Defines functions to draw geometrical shapes by creating meshes.
#pragma once
// Standard Library
#include <optional>
#include <memory>
// Graphics
#include "pixelfall/engine/graphics/shader.h"
#include "pixelfall/engine/graphics/mesh.h"
#include "pixelfall/engine/graphics/color.h"
// Math
#include "pixelfall/engine/math/vector2.h"

class Painter {
public:
    Painter(Shader& shader) : shader(shader) {}

    // Methods
    void drawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color);

private:
    Shader& shader;
};