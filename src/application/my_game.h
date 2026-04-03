// Overwrites the main functions defined in the engine to be used for this specific application.

#pragma once
#include <optional>
#include "engine/core/application.h"
#include "engine/core/shader.h"

class MyGame : public Application {
public:
    bool onStart(std::filesystem::path projectPath)  override;
    void onUpdate() override;
    void onRender() override;

private:
    std::optional<Shader> basicShader;
};