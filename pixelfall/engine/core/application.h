// Virtual class that an application can override to gain access to the basic engine functions.

#pragma once
// Standard Library
#include <memory>
#include <filesystem>
#include <optional>
#include <string>
// Core
#include "pixelfall/engine/core/window.h"
// Graphics
#include "pixelfall/engine/graphics/shader.h"
#include "pixelfall/engine/graphics/painter.h"
// Configuration
#include "pixelfall/engine/config/application_config.h"

class Application {
public:
    static std::unique_ptr<Application> create();
    virtual ~Application() = default;

    // Getters
    virtual std::string getTitle() const { return appTitle; };
    virtual std::string getIdentifier() const { return appIdentifier; };
    virtual std::string getVersion() const { return appVersion; };

    // Methods
    void onSetup(std::filesystem::path enginePath, Window& appWindow);
    virtual bool onStart() { return true; }
    virtual void onUpdate() {}
    virtual void onRender() {}

protected:
    std::string appTitle{application::defaults::appTitle.data()};
    std::string appIdentifier{application::defaults::appIdentifier.data()};
    std::string appVersion{application::defaults::appVersion.data()};
    std::optional<Shader> defaultShader;
    std::optional<Painter> painter;
    Window* appWindow = nullptr;
};