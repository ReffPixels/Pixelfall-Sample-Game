// Implementation of App Window for SDL

#include "core/engine_window.h"
#include <SDL3/SDL.h>

struct EngineWindow::PlatformComponents {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};

EngineWindow::EngineWindow(
    // Values inherited from Base Class (Widget)
    std::string windowTitle,
    Vector2Int physicalSize,
    Vector2Int minWindowSize,
    float minAspectRatio,
    float maxAspectRatio,
    float dprScale,
    window::PresentationMode presentationMode
    ) :
    windowTitle(windowTitle),
    physicalSize(physicalSize),
    minWindowSize(minWindowSize),
    minAspectRatio(minAspectRatio),
    maxAspectRatio(maxAspectRatio),
    dprScale(dprScale),
    presentationMode(presentationMode),
    platformComponents(new PlatformComponents()) {
};

EngineWindow::~EngineWindow() {
    if (platformComponents->window) SDL_DestroyWindow(platformComponents->window);
    if (platformComponents->renderer) SDL_DestroyRenderer(platformComponents->renderer);
    delete platformComponents;
}

bool EngineWindow::init() {
    if (!SDL_CreateWindowAndRenderer(
        windowTitle.c_str(),
        physicalSize.x,
        physicalSize.y,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE,
        &platformComponents->window,
        &platformComponents->renderer)) {
        return false;
    }

    SDL_SetWindowMinimumSize(platformComponents->window, minWindowSize.x, minWindowSize.y);
    SDL_SetWindowAspectRatio(platformComponents->window, minAspectRatio, maxAspectRatio);
    return true;
}

void EngineWindow::clear() {
    SDL_SetRenderDrawColorFloat(platformComponents->renderer, 1.0f, 1.0f, 1.0f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(platformComponents->renderer);
}

void EngineWindow::present() {
    SDL_RenderPresent(platformComponents->renderer);
}

void EngineWindow::setWindowTitle(std::string windowTitle) {
    this->windowTitle = windowTitle;
    if (platformComponents->window)
        SDL_SetWindowTitle(platformComponents->window, windowTitle.c_str());
}

void EngineWindow::recalculate() {
    // TODO
}
