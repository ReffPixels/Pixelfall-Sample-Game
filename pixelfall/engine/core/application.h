// Virtual class that an application can override to gain access to the basic engine functions.

#pragma once
// Standard Library
#include <memory>
#include <filesystem>
#include <optional>
// Core
#include "pixelfall/engine/core/window.h"
// Graphics
#include "pixelfall/engine/graphics/shader.h"
#include "pixelfall/engine/graphics/painter.h"

class Application {
public:
    static std::unique_ptr<Application> create();
    virtual ~Application() = default;

    void setup(std::filesystem::path enginePath, Window& appWindow);

    virtual bool onStart() { return true; }
    virtual void onUpdate() {}
    virtual void onRender() {}

protected:
    std::optional<Shader> defaultShader;
    std::optional<Painter> painter;
    Window* appWindow = nullptr;
};