// Implementation of application.h

#include "pixelfall/engine/core/application.h"

void Application::onSetup(std::filesystem::path enginePath, Window& appWindow) {
    this->appWindow = &appWindow;
    defaultShader.emplace(
        (enginePath / "assets/shaders/vertex_shader.glsl"),
        (enginePath / "assets/shaders/fragment_shader.glsl")
    );
    painter.emplace(*defaultShader, appWindow);
}