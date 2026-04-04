// Defines the Loop of this application.

#include "application/my_game.h"
// Output to console
#include <iostream>
// Shaders
#include <glad/glad.h>

std::unique_ptr<Application> Application::create() {
    return std::make_unique<MyGame>();
}

bool MyGame::onStart() {
    // Debug
    std::cout << "START" << std::endl;

    return true;
}

void MyGame::onUpdate() {
}

void MyGame::onRender() {
    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    painter->drawTriangle(Vector2(-0.5f, -0.5f), Vector2(-0.25f, 0.5f), Vector2(0.0f, -0.5f), Color::fromHexcode("#ff0000"));
    painter->drawTriangle(Vector2(0.5f, 0.5f), Vector2(0.25f, -0.5f), Vector2(0.0f, 0.5f), Color::fromHexcode("#ffff00"));
}
