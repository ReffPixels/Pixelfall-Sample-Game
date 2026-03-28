// Implementation of engine.h - This is where the steps of the main loop are defined.

#include "engine/core/engine.h"
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

bool Engine::startApplication(Application* application) {
    this->application = application;
    if (!application->onStart()) {
        std::cout << "ERROR: Failed to create application" << std::endl;
        isRunning = false;
    }
    else {
        isRunning = true;
    }
    return isRunning;
}

// Main loop. This iterates every frame and executes the application's onUpdate()
bool Engine::update() {

    // Update
    application->onUpdate();
    window->updateWindowData();

    // Rendering
    window->clear();
    application->onRender();
    window->debug();
    window->present();

    return isRunning;
}

Engine::~Engine() {
    delete application;
}