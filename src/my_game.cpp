// Defines the loop of this specific application.

#include "my_game.h"
// Standard Library
#include <iostream>

// Create Game
PIXELFALL_APPLICATION(MyGame);

// Called on the first frame
bool MyGame::onStart() {
    // Set Metadata
    appWindow->setWindowTitle("My awesome game");
    appTitle = "My awesome game";
    appVersion = "0.1";
    appIdentifier = "myAwesomeGame";

    return true;
}

// Called every frame
void MyGame::onUpdate() {
}

// Handles draw calls
void MyGame::onRender() {
    // Background
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getLogicalSize().x, appWindow->getLogicalSize().y),
        Color::fromHexcode("#ccc")
        );
    painter->drawTriangle(
        Vector2(0, 0),
        Vector2(1280, 0),
        Vector2(640, 720),
        Color::fromHexcode("#eee")
    );
    painter->drawCircle(
        Vector2(200, 200),
        90.0f,
        Color::fromHexcode("#f005")
    );
    painter->drawArc(
        Vector2(400, 200),
        90.0f,
        0,
        90,
        Color::fromHexcode("#000")
    );
    painter->drawRegularPolygon(
        Vector2(900, 200),
        140.0f,
        Color::fromHexcode("#f0f"),
        7
    );
}
