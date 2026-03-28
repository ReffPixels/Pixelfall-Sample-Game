// Implementation of App Window for SDL

#include "core/engine_window.h"
#include <SDL3/SDL.h>

// Define components specific to the platform
// SDL needs an SDL_Window pointer and an SDL_Renderer pointer
struct EngineWindow::PlatformComponents {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

// Constructor
EngineWindow::EngineWindow(
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

// Executes when the window is initialized by the engine.
// In the case of SDL, we need to create the window and renderer with its default values.
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
    updateWindowData();
    EngineWindow::setPresentationMode(presentationMode);
    return true;
}

// Clears the window to a flat background colour so we can render onto a clean surface.
void EngineWindow::clear() {
    SDL_SetRenderDrawColorFloat(platformComponents->renderer, 1.0f, 1.0f, 1.0f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(platformComponents->renderer);
}

// Presents the new render with whatever we've drawn onto it.
void EngineWindow::present() {
    SDL_RenderPresent(platformComponents->renderer);
}

// Changes the title displayed on the window bar.
void EngineWindow::setWindowTitle(std::string windowTitle) {
    this->windowTitle = windowTitle;
    if (platformComponents->window)
        SDL_SetWindowTitle(platformComponents->window, windowTitle.c_str());
}

// Recalculates the variable information of the window, like its size or aspect ratio.
void EngineWindow::updateWindowData() {

    // Get current physical size of the window
    SDL_GetWindowSize(platformComponents->window, &physicalSize.x, &physicalSize.y);

    // Get current aspect ratio
    aspectRatio = (float)physicalSize.x / (float)physicalSize.y;

    // Get current scale (How the current window size differs from the base window size)
    scale = Vector2(
        (float)physicalSize.x / window::default::referenceSize.x,
        (float)physicalSize.y / window::default::referenceSize.y
    );

    // Update Render Scale
    SDL_SetRenderScale(platformComponents->renderer, dprScale, dprScale);

    // Update Logical Size (Also updates logical presentation mode)
    updateLogicalSize();
}

// Calculates the logical size depending on the presentation mode.
void EngineWindow::updateLogicalSize() {
    switch (presentationMode) {
    case window::PresentationMode::Free:
        logicalSize = physicalSize;
        SDL_SetRenderLogicalPresentation(
            platformComponents->renderer,
            0, 0, SDL_LOGICAL_PRESENTATION_DISABLED
        );
        break;
    case window::PresentationMode::Letterbox:
        logicalSize = window::default::referenceSize;
        SDL_SetRenderLogicalPresentation(
            platformComponents->renderer,
            window::default::referenceSize.x,
            window::default::referenceSize.y,
            SDL_LOGICAL_PRESENTATION_LETTERBOX
        );
        break;
    case window::PresentationMode::Crop:
        logicalSize = window::default::referenceSize;
        SDL_SetRenderLogicalPresentation(
            platformComponents->renderer,
            window::default::referenceSize.x,
            window::default::referenceSize.y,
            SDL_LOGICAL_PRESENTATION_OVERSCAN
        );
        break;
    case window::PresentationMode::Stretch:
        logicalSize = window::default::referenceSize;
        SDL_SetRenderLogicalPresentation(
            platformComponents->renderer,
            window::default::referenceSize.x,
            window::default::referenceSize.y,
            SDL_LOGICAL_PRESENTATION_STRETCH
        );
        break;
    case window::PresentationMode::Expand: {
        float smallestScale = std::min(scale.x, scale.y);
        logicalSize = {(int)(physicalSize.x / smallestScale), (int)(physicalSize.y / smallestScale)};
        SDL_SetRenderLogicalPresentation(
            platformComponents->renderer,
            logicalSize.x,
            logicalSize.y,
            SDL_LOGICAL_PRESENTATION_STRETCH
        );
        break;}
    case window::PresentationMode::ExpandHorizontal: {
        logicalSize = {(int)(physicalSize.x / scale.y), (int)(physicalSize.y / scale.y)};
        SDL_SetRenderLogicalPresentation(
            platformComponents->renderer,
            logicalSize.x,
            logicalSize.y,
            SDL_LOGICAL_PRESENTATION_STRETCH
        );
        break;
    }
    case window::PresentationMode::ExpandVertical: {
        logicalSize = {(int)(physicalSize.x / scale.x), (int)(physicalSize.y / scale.x)};
        SDL_SetRenderLogicalPresentation(
            platformComponents->renderer,
            logicalSize.x,
            logicalSize.y,
            SDL_LOGICAL_PRESENTATION_STRETCH
        );
        break;
    }
    default:
        logicalSize = physicalSize;
        SDL_SetRenderLogicalPresentation(
            platformComponents->renderer,
            0, 0, SDL_LOGICAL_PRESENTATION_DISABLED
        );
    }
}

void EngineWindow::debug() {
    SDL_SetRenderDrawColorFloat(platformComponents->renderer, 1.0f, 0.0f, 0.0f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_FRect rect {10, 10, (float)window::default::referenceSize.x - 20.0f, (float)window::default::referenceSize.y - 20.0f};
    SDL_RenderFillRect(platformComponents->renderer, &rect);
}

// Destructor
EngineWindow::~EngineWindow() {
    if (platformComponents->window) SDL_DestroyWindow(platformComponents->window);
    if (platformComponents->renderer) SDL_DestroyRenderer(platformComponents->renderer);
    delete platformComponents;
}
