// This defines the start point of the application, creates the window,
// iterates the main loop and handles closing the application.

// Use SDL3 Callbacks instead of main()
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
// Engine
#include "core/engine.h"
#include "config/engine_config.h"
#include "core/engine_window.h"
// Application
#include "application/my_game.h"

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {

    SDL_SetAppMetadata(
        engine::appTitle.data(),
        engine::appVersion.data(),
        engine::appIdentifier.data());

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL_ttf: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create the Engine (It owns the window and renderer that SDL needs) 
    auto *engine = new Engine();
    if (!engine->init()) {
        delete engine;
        SDL_Log("Couldn't create engine: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create the application (Owned by Engine)
    if (!engine->startApplication(new MyGame())) {
        delete engine;
        SDL_Log("Couldn't create application: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = engine;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    return static_cast<Engine *>(appstate)->update()
        ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    delete static_cast<Engine*>(appstate);
    TTF_Quit();
    SDL_Quit();
}