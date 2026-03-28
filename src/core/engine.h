#pragma once
// Basic
#include <filesystem>
// Engine Components
#include "core/engine_window.h"
#include "core/application.h"

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
    EngineWindow window;

    // This application will be created by the platform and owned by Engine.
    Application *application = nullptr;
};