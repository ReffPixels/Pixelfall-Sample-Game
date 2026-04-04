// Defines the loop of this specific application.

#include "application/my_game.h"
// Standard Library
#include <iostream>

// Create Game
std::unique_ptr<Application> Application::create() {
    return std::make_unique<MyGame>();
}

bool MyGame::onStart() {
    std::cout << "START" << std::endl;

    return true;
}

void MyGame::onUpdate() {
}

void MyGame::onRender() {
    painter->drawTriangle(Vector2(-0.5f, -0.5f), Vector2(-0.25f, 0.5f), Vector2(0.0f, -0.5f), Color::fromHexcode("#ff0000"));
    painter->drawTriangle(Vector2(0.5f, 0.5f), Vector2(0.25f, -0.5f), Vector2(0.0f, 0.5f), Color::fromHexcode("#ffff00"));
}
