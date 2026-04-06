// Implementation of application.h

#include "pixelfall/engine/core/application.h"
#include "pixelfall/engine/config/shader_config.h"

// Called by the engine before onStart, this is a private engine function that sets up all the necessary 
// components of the application before the user defined behaviours happen (onStart, onUpdate, etc)
void Application::onSetup(std::filesystem::path enginePath, Window& appWindow, std::filesystem::path projectPath) {
    // Set components
    this->appWindow = &appWindow;
    this->projectPath = projectPath;

    geometryShader.emplace(
        (enginePath / shader::defaults::geometryVertShader.data()),
        (enginePath / shader::defaults::geometryFragShader.data())
    );
    spriteShader.emplace(
        (enginePath / shader::defaults::spriteVertShader.data()),
        (enginePath / shader::defaults::spriteFragShader.data())
    );
    msaaScreenShader.emplace(
        (enginePath / shader::defaults::screenVertShader.data()),
        (enginePath / shader::defaults::msaaFragShader.data())
    );
    fxaaScreenShader.emplace(
        (enginePath / shader::defaults::screenVertShader.data()),
        (enginePath / shader::defaults::fxaaFragShader.data())
    );
    // Use FXAA only if the driver did not grant hardware MSAA
    Shader& activeScreenShader = appWindow.isMSAAEnabled() ? *msaaScreenShader : *fxaaScreenShader;
    painter.emplace(*geometryShader, *spriteShader, activeScreenShader, appWindow);
}