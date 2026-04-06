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
    // Getters
    virtual std::string getTitle() const { return appTitle; };
    virtual std::string getIdentifier() const { return appIdentifier; };
    virtual std::string getVersion() const { return appVersion; };
    Painter& getPainter() { return *painter; };

    // Methods
    void onSetup(std::filesystem::path enginePath, Window& appWindow, std::filesystem::path projectPath);
    virtual bool onStart() { return true; }
    virtual void onUpdate() {}
    virtual void onRender() {}

    // Destroyers
    virtual ~Application() = default;

protected:
    // Metadataa
    std::string appTitle{application::defaults::appTitle.data()};
    std::string appIdentifier{application::defaults::appIdentifier.data()};
    std::string appVersion{application::defaults::appVersion.data()};

    // Shaders
    std::optional<Shader> geometryShader;
    std::optional<Shader> fxaaScreenShader;
    std::optional<Shader> msaaScreenShader;
    std::optional<Shader> spriteShader;

    // Engine Components
    Window* appWindow = nullptr;
    std::optional<Painter> painter;

    // Other tools
    std::filesystem::path projectPath;
};

// Implemented by the user via PIXELFALL_APPLICATION(ApplicationClass)
std::unique_ptr<Application> createApplication();

// This is in the user's entry point to the engine. It creates an instance of their application class, 
// which overrides this virtual application class (Granting them access to the engine loop and functions)
#define PIXELFALL_APPLICATION(ApplicationClass) \
    std::unique_ptr<Application> createApplication() { \
        return std::make_unique<ApplicationClass>(); \
    }