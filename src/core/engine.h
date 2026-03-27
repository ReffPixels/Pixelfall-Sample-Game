#pragma once
// Basic
#include <filesystem>
// Engine Components
#include "core/app_window.h"

class Engine {
public:
    // Methods
    bool init();
    bool iterate();

    // Destructor
    ~Engine();

private:
    bool isRunning = false;
    std::filesystem::path projectPath;

    // Engine Components
    AppWindow window;
};