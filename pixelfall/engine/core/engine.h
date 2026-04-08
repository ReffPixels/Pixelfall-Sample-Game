// Defines the Engine class, which controls the state of the application and owns the main loop.

#pragma once
// Standard Library
#include <filesystem>
#include <memory>
// Engine Components
#include "pixelfall/engine/core/window.h"
#include "pixelfall/engine/core/clock.h"
#include "pixelfall/engine/input/input_manager.h"
// Application
#include "pixelfall/engine/core/application.h"

class Engine {
public:
    // Getters
    Application* getApplication() { return application.get(); };
    Window* getWindow() { return window.get(); };
    Clock* getClock() { return clock.get(); };
    InputManager* getInputManager() { return inputManager.get(); };
    std::filesystem::path getEnginePath() { return enginePath; };
    std::filesystem::path getProjectPath() { return projectPath; };

    // Setters
    void setProjectPath(std::filesystem::path projectPath) { this->projectPath = projectPath; };
    void setEnginePath(std::filesystem::path enginePath) { this->enginePath = enginePath; };

    // Methods
    bool init();
    bool startApplication(std::unique_ptr<Application> application);
    bool update();

    // Destructor
    ~Engine();

private:
    // isRunning is set to true on init(), and if set to false it will stop the engine.
    bool isRunning = false;

    // Engine Components
    std::unique_ptr<Window> window;
    std::unique_ptr<Clock> clock;
    std::unique_ptr<InputManager> inputManager;

    // Store paths
    std::filesystem::path projectPath;
    std::filesystem::path enginePath;

    // This application will be created by the platform and owned by Engine.
    std::unique_ptr<Application> application;
};