#include "core/engine.h"
// Basic
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

// Iterates every frame and executes application onUpdate()
bool Engine::update() {

    application->onUpdate();
    window.clear();
    application->onRender();
    window.updateWindowData();
    window.debug();
    window.present();

    return isRunning;
}

Engine::~Engine() {
    delete application;
}