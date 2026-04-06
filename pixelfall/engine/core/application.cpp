// Implementation of application.h

#include "pixelfall/engine/core/application.h"
#include "pixelfall/engine/config/shader_config.h"

void Application::onSetup(std::filesystem::path enginePath, Window& appWindow) {
    this->appWindow = &appWindow;
    defaultShader.emplace(
        (enginePath / shader::defaults::defaultVertexShader.data()),
        (enginePath / shader::defaults::defaultFragmentShader.data())
    );
    screenShader.emplace(
        (enginePath / shader::defaults::screenVertexShader.data()),
        (enginePath / shader::defaults::fxaaFragmentShader.data())
    );
    painter.emplace(*defaultShader, *screenShader, appWindow);
}