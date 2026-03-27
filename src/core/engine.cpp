#include "core/engine.h"
#include <iostream>

bool Engine::init() {
    if (!window.init()) {
        std::cout << "ERROR: Failed to create engine window" << std::endl;
        isRunning = false;
        return false;
    }
    isRunning = true;
    return true;
}

bool Engine::iterate() {
    window.clear();
    window.present();
    return isRunning;
}

Engine::~Engine() {
}