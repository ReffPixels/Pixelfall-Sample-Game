// Implementation of application.h

#include "pixelfall/engine/core/application.h"
#include "pixelfall/engine/config/shader_config.h"

void Application::onSetup(std::filesystem::path enginePath, Window& appWindow) {
    this->appWindow = &appWindow;

    geometryShader.emplace(
        (enginePath / shader::defaults::geometryVertShader.data()),
        (enginePath / shader::defaults::geometryFragShader.data())
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
    painter.emplace(*geometryShader, activeScreenShader, appWindow);
}