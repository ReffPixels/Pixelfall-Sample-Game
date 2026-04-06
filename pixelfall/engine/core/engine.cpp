// Implementation of engine.h

#include "pixelfall/engine/core/engine.h"
#include <iostream>

// Creates the engine and executes application onStart()
bool Engine::init() {
    window = std::make_unique<Window>();
    clock = std::make_unique<Clock>();

    if (!window->init()) {
        std::cout << "ERROR: Failed to create engine window" << std::endl;
        isRunning = false;
        return false;
    }
    if (!clock->init()) {
        std::cout << "ERROR: Failed to create engine clock" << std::endl;
        isRunning = false;
        return false;
    }

    // Engine creation success
    isRunning = true;
    return true;
}

// Attempts to create and start the application
bool Engine::startApplication(std::unique_ptr<Application> application) {
    this->application = std::move(application);
    this->application->onSetup(enginePath, *window, *clock, projectPath);
    if (!this->application->onStart()) {
        std::cout << "ERROR: Failed to create application" << std::endl;
        isRunning = false;
        return false;
    }

    isRunning = true;
    return true;
}

// Main loop. This iterates every frame and executes the application's onUpdate()
bool Engine::update() {

    // Update
    application->onUpdate();
    window->updateWindowData();

    // Rendering
    window->clear();
    application->getPainter().begin();
    application->onRender();
    application->getPainter().end();
    window->present();

    return isRunning;
}

// Destructor
Engine::~Engine() {
}