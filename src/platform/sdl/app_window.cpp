// Implementation of App Window for SDL

#include "core/app_window.h"
#include <SDL3/SDL.h>

struct AppWindow::AppWindowSDL {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};

AppWindow::AppWindow(
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
    appWindowSDL(new AppWindowSDL()){
};

AppWindow::~AppWindow() {
    if (appWindowSDL->window) SDL_DestroyWindow(appWindowSDL->window);
    delete appWindowSDL;
}

bool AppWindow::init() {
    if (!SDL_CreateWindowAndRenderer(
        windowTitle.c_str(),
        physicalSize.x,
        physicalSize.y,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE,
        &appWindowSDL->window,
        &appWindowSDL->renderer)) {
        return false;
    }

    SDL_SetWindowMinimumSize(appWindowSDL->window, minWindowSize.x, minWindowSize.y);
    SDL_SetWindowAspectRatio(appWindowSDL->window, minAspectRatio, maxAspectRatio);
    return true;
}

void AppWindow::clear() {
    SDL_SetRenderDrawColorFloat(appWindowSDL->renderer, 1.0f, 1.0f, 1.0f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(appWindowSDL->renderer);
}

void AppWindow::present() {
    SDL_RenderPresent(appWindowSDL->renderer);
}

void AppWindow::setWindowTitle(std::string windowTitle) {
    this->windowTitle = windowTitle;
    if (appWindowSDL->window)
        SDL_SetWindowTitle(appWindowSDL->window, windowTitle.c_str());
}

void AppWindow::recalculate() {
    // TODO
}
