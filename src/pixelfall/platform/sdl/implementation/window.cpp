// Implementation of window.h for SDL.

#include "pixelfall/engine/core/window.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>

// Define components specific to the platform (SDL and OpenGL)
struct Window::PlatformComponents {
    SDL_Window* window = nullptr;
    SDL_GLContext context{};
};

// Constructor
Window::Window(
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
// In the case of SDL and OpenGL, we need to define the OpenGL settings before creating the window, 
// and then create an OpenGL context and bind it to the window.
bool Window::init() {

    // Settings for OpenGL Context (Version 3.3, Core)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create the window with an OpenGL Flag
    if (!(platformComponents->window = SDL_CreateWindow(
        windowTitle.c_str(),
        physicalSize.x,
        physicalSize.y,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL))) {
        // ERROR
        return false;
    }

    // Create the OpenGL context for the window.
    if (!(platformComponents->context
        = SDL_GL_CreateContext(platformComponents->window))) {
        // ERROR
        return false;
    };


    // Load OpenGL functions via GLAD
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    // Define Window properties
    SDL_SetWindowMinimumSize(platformComponents->window, minWindowSize.x, minWindowSize.y);
    SDL_SetWindowAspectRatio(platformComponents->window, minAspectRatio, maxAspectRatio);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    updateWindowData();

    return true;
}

// Clears the window to a flat background colour so we can render onto a clean surface.
void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

// Presents the new render with whatever we've drawn onto it.
void Window::present() {
    SDL_GL_SwapWindow(platformComponents->window);
}

// Changes the title displayed on the window bar.
void Window::setWindowTitle(std::string windowTitle) {
    this->windowTitle = windowTitle;
    if (platformComponents->window)
        SDL_SetWindowTitle(platformComponents->window, windowTitle.c_str());
}

void Window::setPresentationMode(window::PresentationMode presentationMode) {
    this->presentationMode = presentationMode;
    updateLogicalPresentation();
}

// Recalculates the variable information of the window, like its size or aspect ratio.
void Window::updateWindowData() {

    // Get current physical size of the window (In Pixels version is aware of DPR scale)
    SDL_GetWindowSizeInPixels(platformComponents->window, &physicalSize.x, &physicalSize.y);

    // Get current aspect ratio
    aspectRatio = (float)physicalSize.x / (float)physicalSize.y;

    // Get current scale (Relative to the reference size)
    scale = Vector2(
        (float)physicalSize.x / window::defaults::referenceSize.x,
        (float)physicalSize.y / window::defaults::referenceSize.y
    );

    // Set OpenGL viewport to fill the window
    glViewport(0, 0, physicalSize.x, physicalSize.y);

    // Updates Logical Presentation (This process also updates logical size to match the new presentation)
    updateLogicalPresentation();
}

// Calculates the logical size depending on the presentation mode and updates presentation mode.
void Window::updateLogicalPresentation() {
    // Simplified for now to set up OpenGL
    logicalSize = physicalSize;

    // switch (presentationMode) {
    // case window::PresentationMode::Free:
    //     logicalSize = physicalSize;
    //     SDL_SetRenderLogicalPresentation(
    //         platformComponents->renderer,
    //         0, 0, SDL_LOGICAL_PRESENTATION_DISABLED
    //     );
    //     break;
    // case window::PresentationMode::Letterbox:
    //     logicalSize = window::defaults::referenceSize;
    //     SDL_SetRenderLogicalPresentation(
    //         platformComponents->renderer,
    //         window::defaults::referenceSize.x,
    //         window::defaults::referenceSize.y,
    //         SDL_LOGICAL_PRESENTATION_LETTERBOX
    //     );
    //     break;
    // case window::PresentationMode::Crop:
    //     logicalSize = window::defaults::referenceSize;
    //     SDL_SetRenderLogicalPresentation(
    //         platformComponents->renderer,
    //         window::defaults::referenceSize.x,
    //         window::defaults::referenceSize.y,
    //         SDL_LOGICAL_PRESENTATION_OVERSCAN
    //     );
    //     break;
    // case window::PresentationMode::Stretch:
    //     logicalSize = window::defaults::referenceSize;
    //     SDL_SetRenderLogicalPresentation(
    //         platformComponents->renderer,
    //         window::defaults::referenceSize.x,
    //         window::defaults::referenceSize.y,
    //         SDL_LOGICAL_PRESENTATION_STRETCH
    //     );
    //     break;
    // case window::PresentationMode::Expand: {
    //     float smallestScale = std::min(scale.x, scale.y);
    //     logicalSize = {(int)(physicalSize.x / smallestScale), (int)(physicalSize.y / smallestScale)};
    //     SDL_SetRenderLogicalPresentation(
    //         platformComponents->renderer,
    //         logicalSize.x,
    //         logicalSize.y,
    //         SDL_LOGICAL_PRESENTATION_STRETCH
    //     );
    //     break;
    // }
    // case window::PresentationMode::ExpandHorizontal: {
    //     logicalSize = {(int)(physicalSize.x / scale.y), (int)(physicalSize.y / scale.y)};
    //     SDL_SetRenderLogicalPresentation(
    //         platformComponents->renderer,
    //         logicalSize.x,
    //         logicalSize.y,
    //         SDL_LOGICAL_PRESENTATION_STRETCH
    //     );
    //     break;
    // }
    // case window::PresentationMode::ExpandVertical: {
    //     logicalSize = {(int)(physicalSize.x / scale.x), (int)(physicalSize.y / scale.x)};
    //     SDL_SetRenderLogicalPresentation(
    //         platformComponents->renderer,
    //         logicalSize.x,
    //         logicalSize.y,
    //         SDL_LOGICAL_PRESENTATION_STRETCH
    //     );
    //     break;
    // }
    // default:
    //     logicalSize = physicalSize;
    //     SDL_SetRenderLogicalPresentation(
    //         platformComponents->renderer,
    //         0, 0, SDL_LOGICAL_PRESENTATION_DISABLED
    //     );
    // }
}

void Window::debug() {
    // SDL_SetRenderDrawColorFloat(platformComponents->renderer, 1.0f, 0.0f, 0.0f, SDL_ALPHA_OPAQUE_FLOAT);
    // SDL_FRect rect{10, 10, (float)window::defaults::referenceSize.x - 20.0f, (float)window::defaults::referenceSize.y - 20.0f};
    // SDL_RenderFillRect(platformComponents->renderer, &rect);
}

// Destructor
Window::~Window() {
    if (platformComponents->context) SDL_GL_DestroyContext(platformComponents->context);
    if (platformComponents->window) SDL_DestroyWindow(platformComponents->window);
    delete platformComponents;
}
