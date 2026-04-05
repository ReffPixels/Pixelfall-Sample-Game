// Allows drawing geometrical shapes by creating meshes.

#pragma once
// Standard Library
#include <vector>
// Graphics
#include "pixelfall/engine/graphics/shader.h"
#include "pixelfall/engine/graphics/color.h"
// Core
#include "pixelfall/engine/core/window.h"
// Math
#include "pixelfall/engine/math/vector2.h"
// Config
#include "pixelfall/engine/config/painter_config.h"

class Painter {
public:
    Painter(Shader& shader, Window& window)
        : shader(shader), window(window) {}

    // Methods
    void begin();
    void drawPolygon(const std::vector<Vector2>& points, Color color);
    void drawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color);
    void drawQuad(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Color color);
    void drawRectangle(Vector2 position, Vector2 size, Color color);
    void drawArc(Vector2 center, float radius, float startAngle, float endAngle, Color color,
        int segments = painter::defaults::arcSegments);
    void drawCircle(Vector2 center, float radius, Color color);
    void drawRegularPolygon(Vector2 center, float radius, Color color, int segments);

private:
    Shader& shader;
    Window& window;
};