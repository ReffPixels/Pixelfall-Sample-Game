// Implementation of time.h for SDL.

#include "pixelfall/engine/core/clock.h"
#include <SDL3/SDL.h>

// Called when the engine is created
bool Clock::init() {
    // Set a start value to avoid update dividing by 0 on the first frame.
    lastFrameStartTime = SDL_GetPerformanceCounter();
    return true;
}

// Calculates this frame's start time, delta time, FPS and the previous frame's start time.
void Clock::update() {
    frameStartTime = SDL_GetPerformanceCounter();

    // Calculate Delta Time in seconds
    deltaTime = (double)(frameStartTime - lastFrameStartTime) / SDL_GetPerformanceFrequency();
    lastFrameStartTime = frameStartTime;

    // Calculate FPS
    fps = 1.0 / deltaTime;
}

// Resets time values to 0
void Clock::reset() {
    frameStartTime = 0;
    lastFrameStartTime = 0;
    deltaTime = 0.0;
    fps = 0.0;
}

// Makes the program wait in order to limit the framerate to the chosen fps. Must be called at the end of the frame.
void Clock::limitFPS() {
    Uint64 frameEndTime = SDL_GetPerformanceCounter();
    double frameDuration = (double)(frameEndTime - frameStartTime) / SDL_GetPerformanceFrequency();
    double targetFrameDuration = 1 / fpsLimit;

    if (frameDuration < targetFrameDuration) {
        SDL_Delay((Uint32)((targetFrameDuration - frameDuration) * 1000.0));
    }
}