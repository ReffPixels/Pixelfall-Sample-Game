#include "pixelfall/engine/graphics/painter.h"

void Painter::drawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color) {
    std::vector<float> vertices = {
        a.x, a.y, 0.0f,
        b.x, b.y, 0.0f,
        c.x, c.y, 0.0f
    };
    std::vector<unsigned int> indices = {0, 1, 2};

    Mesh mesh(vertices, indices);
    shader.use();
    shader.setUniformVec4("customColor", color.r, color.g, color.b, color.a);
    mesh.draw();
}