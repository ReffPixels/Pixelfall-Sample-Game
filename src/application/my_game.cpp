// Defines the Loop of this application.

#include "application/my_game.h"
#include <memory>

std::unique_ptr<Application> Application::create() {
    return std::make_unique<MyGame>();
}


bool MyGame::onStart() {
    return true;
}

void MyGame::onUpdate() {
}

void MyGame::onRender() {
}
