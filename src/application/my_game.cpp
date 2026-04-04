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
    painter->drawTriangle(Vector2(1280, 0), Vector2(2560, 0), Vector2(1280, 720), Color::fromHexcode("#00ff22"));
    painter->drawTriangle(Vector2(2560, 0), Vector2(1280, 720), Vector2(2560, 720), Color::fromHexcode("#00ff22"));
    painter->drawTriangle(Vector2(0, 0), Vector2(1280, 0), Vector2(0, 720), Color::fromHexcode("#ffffff"));
    painter->drawTriangle(Vector2(1280, 0), Vector2(0, 720), Vector2(1280, 720), Color::fromHexcode("#ffffff"));
    painter->drawTriangle(Vector2(0, 0), Vector2(1280, 0), Vector2(640, 720), Color::fromHexcode("#ff0000"));
    painter->drawTriangle(Vector2(400, 400), Vector2(500, 300), Vector2(500, 400), Color::fromHexcode("#ffff00"));
}
