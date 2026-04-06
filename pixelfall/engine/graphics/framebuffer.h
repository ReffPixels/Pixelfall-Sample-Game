// Defines a framebuffer that the painter renders into, then blits to the screen.
// The actual implementation is platform specific.

#pragma once
// Standard Library
#include <memory>
// Math
#include "pixelfall/engine/math/vector2.h"

class Framebuffer {
public:
    Framebuffer(Vector2Int size);

    // Methods
    void bind();    // Bind FBO — subsequent draws go here
    void unbind();  // Unbind FBO and restore full-window viewport for the blit pass
    void drawQuad(); // Draw the fullscreen quad (used by Painter::end to blit with FXAA)

    // Destroyers
    ~Framebuffer();

private:
    Vector2Int size;

    // Platform Implementation (Each platform fills this struct with their necessary components)
    struct PlatformComponents;
    std::unique_ptr<PlatformComponents> platform;
};
