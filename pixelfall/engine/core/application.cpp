// Implementation of application.h

#include "pixelfall/engine/core/application.h"
#include "pixelfall/engine/config/shader_config.h"

// Called by the engine before onStart, this is a private engine function that sets up all the necessary 
// components of the application before the user defined behaviours happen (onStart, onUpdate, etc)
void Application::onSetup(
    std::filesystem::path enginePath, Window& appWindow, Clock& appClock, std::filesystem::path projectPath) {
    // Set components
    this->appWindow = &appWindow;
    this->appClock = &appClock;
    this->projectPath = projectPath;
    textureCache.emplace();

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
    Shader& activeScreenShader = (appWindow.isMSAAEnabled() || !application::autoEnableFXAA) ? *msaaScreenShader : *fxaaScreenShader;
        
    painter.emplace(*geometryShader, *spriteShader, activeScreenShader, appWindow);
}