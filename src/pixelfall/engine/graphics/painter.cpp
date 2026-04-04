// Implementation for painter.h

#include "pixelfall/engine/graphics/painter.h"
// Graphics
#include "pixelfall/engine/graphics/mesh.h"

void Painter::drawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color) {
    std::vector<float> vertices = {
        a.x, a.y, 0.0f,
        b.x, b.y, 0.0f,
        c.x, c.y, 0.0f
    };
    std::vector<unsigned int> indices = {0, 1, 2};

    float w = (float)window.getLogicalSize().x;
    float h = (float)window.getLogicalSize().y;

    // Column-major orthographic matrix: pixel coords (0,0 top-left) -> NDC
    float ortho[16] = {
        2.0f / w,  0.0f,    0.0f, 0.0f,  // column 0
        0.0f,   -2.0f / h,  0.0f, 0.0f,  // column 1
        0.0f,    0.0f,    1.0f, 0.0f,  // column 2
       -1.0f,    1.0f,    0.0f, 1.0f   // column 3
    };

    Mesh mesh(vertices, indices);
    shader.use();
    shader.setUniformMat4("projectionMatrix", ortho);
    shader.setUniformVec4("customColor", color.r, color.g, color.b, color.a);
    mesh.draw();
}