// Defines the loop of this specific application.

#include "my_game.h"
// Standard Library
#include <iostream>

// Create Game
PIXELFALL_APPLICATION(MyGame);

bool MyGame::onStart() {
    // Set Metadata
    appWindow->setWindowTitle("My awesome game");
    appTitle = "My awesome game";
    appVersion = "0.1";
    appIdentifier = "myAwesomeGame";

    return true;
}

void MyGame::onUpdate() {
}

void MyGame::onRender() {
    // Background
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getPhysicalSize().x, appWindow->getPhysicalSize().y),
        Color::fromHexcode("#cccccc")
        );
    painter->drawTriangle(Vector2(0, 0), Vector2(1280, 0), Vector2(640, 720), Color::fromHexcode("#eeeeee"));
    painter->drawCircle(
        Vector2(200, 200),
        90.0f,
        Color::fromHexcode("#ff0000")
    );
    painter->drawArc(
        Vector2(400, 200),
        90.0f,
        0,
        90,
        Color::fromHexcode("#000000")
    );
    painter->drawRegularPolygon(
        Vector2(900, 200),
        140.0f,
        Color::fromHexcode("#ff00ff"),
        7
    );
}
