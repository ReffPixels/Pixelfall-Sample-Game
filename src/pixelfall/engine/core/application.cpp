#include "pixelfall/engine/core/application.h"

void Application::setup(std::filesystem::path projectPath) {
    defaultShader.emplace(
        (projectPath / "assets/shaders/vertex_shader.glsl"),
        (projectPath / "assets/shaders/fragment_shader.glsl")
    );
    painter.emplace(*defaultShader);
}