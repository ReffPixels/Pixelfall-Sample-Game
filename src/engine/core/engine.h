// Defines the Engine class, which controls the state of the application and owns the main loop.

#pragma once
#include <filesystem>
#include "engine/core/window.h"
#include "engine/core/application.h"

class Engine {
public:
    // Methods
    bool init();
    bool startApplication(Application* application);
    bool update();

    // Destructor
    ~Engine();

private:
    // isRunning is set to true on init(), and if set to false it will stop the engine.
    bool isRunning = false;

    // Engine Components
    Window window;

    // This application will be created by the platform and owned by Engine.
    Application *application = nullptr;
};