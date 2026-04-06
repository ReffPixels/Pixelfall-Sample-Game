// Implementation for painter.h

#include "pixelfall/engine/graphics/painter.h"
// Standard Library
#include <iostream>
#include <cmath>
// Graphics
#include "pixelfall/engine/graphics/mesh.h"
// Math
#include "pixelfall/engine/math/numbers.h"
#include "pixelfall/engine/math/tools.h"

// Constructor - Also creates the frame buffer (Needed for anti aliasing)
Painter::Painter(
    Shader& geometryShader,
    Shader& spriteShader,
    Shader& screenShader,
    Window& window)
    :
    geometryShader(geometryShader),
    spriteShader(spriteShader),
    screenShader(screenShader),
    window(window) {

    framebufferSize = window.getPhysicalSize();
    framebuffer = std::make_unique<Framebuffer>(framebufferSize);
}

// Binds the FBO and sets up the geometry shader with the orthographic projection.
// Called by the engine before onRender().
void Painter::begin() {
    float w = (float)window.getLogicalSize().x;
    float h = (float)window.getLogicalSize().y;

    // Orthographic matrix used to transform pixel coordinates to NDC coordinates
    float ortho[16] = {
        2.0f/w,  0.0f,    0.0f,    0.0f,  // column 0, scales X
        0.0f,   -2.0f/h,  0.0f,    0.0f,  // column 1, flips and scales Y
        0.0f,    0.0f,    1.0f,    0.0f,  // column 2, scales Z
       -1.0f,    1.0f,    0.0f,    1.0f   // column 3, moves origin
    };
    // Store matrix so we can reuse it in drawSprite
    std::memcpy(projectionMatrix, ortho, sizeof(projectionMatrix));

    auto physSize = window.getPhysicalSize();
    if (physSize.x != framebufferSize.x || physSize.y != framebufferSize.y) {
        framebufferSize = physSize;
        framebuffer = std::make_unique<Framebuffer>(framebufferSize);
    }

    framebuffer->bind();
    geometryShader.use();
    geometryShader.setUniformMat4("projectionMatrix", ortho);
}

// Unbinds the FBO and blits it to the screen through the FXAA shader.
// Called by the engine after onRender().
void Painter::end() {
    auto physSize = window.getPhysicalSize();
    framebuffer->unbind();

    screenShader.use();
    screenShader.setUniformInt("screenTexture", 0);
    screenShader.setUniformVec2("rcpFrame", 1.0f / physSize.x, 1.0f / physSize.y);

    framebuffer->drawQuad();
}

// Draws a convex polygon with a flat color. Concave polygons are not currently supported.
// This function is used by most geometry draw calls.
void Painter::drawPolygon(const std::vector<Vector2>& points, Color color) {
    geometryShader.use();

    std::vector<float> vertices;
    for (const Vector2& p : points) {
        vertices.push_back(p.x);
        vertices.push_back(p.y);
        vertices.push_back(0.0f);
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 1; i < points.size() - 1; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    Mesh mesh(vertices, indices);
    geometryShader.setUniformVec4("customColor", color.r, color.g, color.b, color.a);
    mesh.draw();
}

// Draws a triangle from 3 points
void Painter::drawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color) {
    drawPolygon({a, b, c}, color);
}

// Draws a quad from 4 points
void Painter::drawQuad(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Color color) {
    drawPolygon({a, b, c, d}, color);
}

// Draws a rectangle from a position, width and height with a flat color
void Painter::drawRectangle(Vector2 position, Vector2 size, Color color) {
    drawQuad(
        position,
        Vector2(position.x + size.x, position.y),
        Vector2(position.x + size.x, position.y + size.y),
        Vector2(position.x, position.y + size.y),
        color
    );
}

// Draws an arc made out of triangular segments starting and ending at any angle (In degrees)
void Painter::drawArc(Vector2 center, float radius, float startAngle, float endAngle, Color color, int segments) {

    // Convert angles from degrees to radians
    startAngle = math::conversions::degreesToRadians(startAngle);
    endAngle = math::conversions::degreesToRadians(endAngle);

    // Store points of the circle polygon
    std::vector<Vector2> points;

    // The center of the circle is Vertex 0
    points.push_back(center);

    // Find Step Size (Angle between each of the segments)
    float stepSize = (endAngle - startAngle) / (float)segments;

    // Find the angle of each arc point
    for (int i = 0; i <= segments; i++) {
        // Find angle of this point. The first item (i = 0) will just be startAngle
        float arcPointAngle = startAngle + stepSize * (float)i;

        // Find the coordinate points in the circumference by using cos and sin
        points.push_back(Vector2(
            center.x + radius * cosf(arcPointAngle),
            center.y - radius * sinf(arcPointAngle) // Using minus to flip the Y axis (More intuitive for the end user)
            ));
    }

    drawPolygon(points, color);
}

// Draws a circle made out of triangular segments (Uses more segments at bigger radius)
void Painter::drawCircle(Vector2 center, float radius, Color color) {

    // Calculate a reasonable amount of segments
    int segments = std::clamp(
        (int)(radius * painter::defaults::circleSegmentsDetail),
        painter::defaults::minCircleSegments,
        painter::defaults::maxCircleSegments
        );
    
    // Draw an arc that starts at 0° and ends at 360°
    drawArc(center, radius, 0.0f, 360.0f, color, segments);
}

// Draws a regular polygon maade out of triangular segments 
// This is equivalent to drawCircle but without the automatic segment calculation
void Painter::drawRegularPolygon(Vector2 center, float radius, Color color, int segments) {

    // Draw an arc that starts at 0° and ends at 360°
    drawArc(center, radius, 0.0f, 360.0f, color, segments);
}

// Draws a mesh with a texture to display an image on the screen. Can also be tinted.
void Painter::drawSprite(Vector2 position, Vector2 size, Texture& texture, Color tint) {
    spriteShader.use();
    spriteShader.setUniformMat4("projectionMatrix", projectionMatrix);
    spriteShader.setUniformInt("customTexture", 0);
    spriteShader.setUniformVec4("tintColor", tint.r, tint.g, tint.b, tint.a);

    texture.bind();

    // Positions of each corner of the texture followed by their UV coordinates
    std::vector<float> vertices = {
        position.x, position.y,
        0.0f, 0.0f, 1.0f,
        position.x + size.x, position.y,
        0.0f, 1.0f, 1.0f,
        position.x + size.x, position.y + size.y,
        0.0f, 1.0f, 0.0f,
        position.x, position.y + size.y,
        0.0f, 0.0f, 0.0f,
    };
    std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

    Mesh mesh(vertices, indices, {{3}, {2}});
    mesh.draw();
}
