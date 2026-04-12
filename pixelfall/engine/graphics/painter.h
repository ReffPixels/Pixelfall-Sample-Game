// Allows drawing geometrical shapes by creating meshes.

#pragma once
// Standard Library
#include <vector>
#include <memory>
// Graphics
#include "pixelfall/engine/graphics/shader.h"
#include "pixelfall/engine/graphics/color.h"
#include "pixelfall/engine/graphics/framebuffer.h"
#include "pixelfall/engine/graphics/texture.h"
#include "pixelfall/engine/graphics/texture_cache.h"
// Core
#include "pixelfall/engine/core/window.h"
// Math
#include "pixelfall/engine/math/vector2.h"
// Config
#include "pixelfall/engine/config/painter_config.h"

class Painter {
public:
    Painter(Shader& geometryShader, Shader& spriteShader, Shader& screenShader,
        Window& window, std::filesystem::path projectPath);

    // Life Cycle
    void begin(); // Called by engine before onRender() — binds FBO and sets up projection
    void end();   // Called by engine after onRender() — applies FXAA and blits to screen

    // Basic shapes
    void drawPolygon(const std::vector<Vector2>& points, Color color);
    void drawTriangle(Vector2 a, Vector2 b, Vector2 c, Color color);
    void drawQuad(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Color color);
    void drawRectangle(Vector2 position, Vector2 size, Color color);
    void drawArc(Vector2 center, float radius, float startAngle, float endAngle, Color color,
        int segments = painter::defaults::arcSegments);
    void drawCircle(Vector2 center, float radius, Color color);
    void drawRegularPolygon(Vector2 center, float radius, Color color, int segments);

    // Textures
    void drawSprite(Vector2 position, Texture& texture,
        Color tint = painter::defaults::textureTint);
    void drawSprite(Vector2 position, Vector2 size, Texture& texture,   // Size overload
        Color tint = painter::defaults::textureTint);

    // Complex Shapes
    void drawRectangleHollow(Vector2 position, Vector2 size, Vector2 innerSize, Color color);
    void drawCircleHollow(Vector2 center, float radius, float innerRadius, Color color);

    // Public Members
    std::unique_ptr<TextureCache> textureCache;
private:
    // Shaders
    Shader& geometryShader;
    Shader& spriteShader;
    Shader& screenShader;

    // Screen
    Window& window;
    std::unique_ptr<Framebuffer> framebuffer;
    Vector2Int framebufferSize;
    float projectionMatrix[16]{};
};