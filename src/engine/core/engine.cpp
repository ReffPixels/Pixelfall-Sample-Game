// Implementation of engine.h - This is where the steps of the main loop are defined.

#include "engine/core/engine.h"
#include <iostream>

// Creates the engine and executes application onStart()
bool Engine::init() {
    if (!window.init()) {
        std::cout << "ERROR: Failed to create engine window" << std::endl;
        isRunning = false;
    }
    else {
        isRunning = true;
    }
    return isRunning;
}

bool Engine::startApplication(Application *application) {
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
    window.updateWindowData();

    // Rendering
    window.clear();
    application->onRender();
    window.debug();
    window.present();

    return isRunning;
}

Engine::~Engine() {
    delete application;
}