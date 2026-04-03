// Overwrites the main functions defined in the engine to be used for this specific application.

#pragma once
#include "engine/core/application.h"
// Shaders
#include "engine/core/shader.h"
#include <optional>

class MyGame : public Application {
public:
    bool onStart(std::filesystem::path projectPath)  override;
    void onUpdate() override;
    void onRender() override;

private:
    std::optional<Shader> basicShader;
};