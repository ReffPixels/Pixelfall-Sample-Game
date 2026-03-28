// Defines the Engine class, which controls the state of the application and owns the main loop.

#pragma once
#include <filesystem>
#include <memory>
#include "engine/core/window.h"
#include "engine/core/clock.h"
#include "engine/core/application.h"

class Engine {
public:
    // Methods
    bool init();
    bool startApplication(std::unique_ptr<Application> application);
    bool update();

    // Getters
    Window* getWindow() { return window.get(); };
    Clock* getClock() { return clock.get(); };

    // Setters
    void setProjectPath(std::filesystem::path projectPath) { this->projectPath = projectPath; };

    // Destructor
    ~Engine();

private:
    // isRunning is set to true on init(), and if set to false it will stop the engine.
    bool isRunning = false;

    // Engine Components
    std::unique_ptr<Window> window;
    std::unique_ptr<Clock> clock;

    // Store path
    std::filesystem::path projectPath;

    // This application will be created by the platform and owned by Engine.
    std::unique_ptr<Application> application;
};