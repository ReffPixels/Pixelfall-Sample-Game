// Implementation of window.h for SDL.

#include "pixelfall/engine/core/window.h"
// Standard Library
#include <algorithm>
// SDL
#include <SDL3/SDL.h>
// OpenGL
#include <glad/glad.h>

// Define components specific to the platform (SDL and OpenGL)
struct Window::PlatformComponents {
    SDL_Window* window = nullptr;
    SDL_GLContext context{};
};

// Constructor
Window::Window(
    std::string windowTitle,
    Vector2Int physicalSize,
    Vector2Int referenceSize,
    Vector2Int minWindowSize,
    float minAspectRatio,
    float maxAspectRatio,
    float dprScale,
    window::PresentationMode presentationMode,
    Color clearColor
    ) :
    windowTitle(windowTitle),
    physicalSize(physicalSize),
    referenceSize(referenceSize),
    minWindowSize(minWindowSize),
    minAspectRatio(minAspectRatio),
    maxAspectRatio(maxAspectRatio),
    dprScale(dprScale),
    presentationMode(presentationMode),
    platform(std::make_unique<PlatformComponents>()),
    clearColor(clearColor) {
};

// Executes when the window is initialized by the engine.
// In the case of SDL and OpenGL, we need to define the OpenGL settings before creating the window, 
// and then create an OpenGL context and bind it to the window.
bool Window::init() {

    // Settings for OpenGL Context (Version 3.3, Core)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Using MSAA (OpenGL Anti Aliasing)
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // Create the window with an OpenGL Flag
    if (!(platform->window = SDL_CreateWindow(
        windowTitle.c_str(),
        physicalSize.x,
        physicalSize.y,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL))) {
        // ERROR
        return false;
    }

    // Create the OpenGL context for the window.
    if (!(platform->context
        = SDL_GL_CreateContext(platform->window))) {
        // ERROR
        return false;
    };

    // Load OpenGL functions via GLAD
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    // Enable blending and standard transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable OpenGL MSAA and check whether the driver actually granted it.
    // If no samples are is not granted, the painter will use FXAA for anti aliasing instead.
    glEnable(GL_MULTISAMPLE);
    int grantedSamples = 0;
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &grantedSamples);
    msaaGranted = grantedSamples > 0;

    // Define Window properties
    SDL_SetWindowMinimumSize(platform->window, minWindowSize.x, minWindowSize.y);
    SDL_SetWindowAspectRatio(platform->window, minAspectRatio, maxAspectRatio);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    updateWindowData();

    return true;
}

// Clears the window to a flat background colour so we can render onto a clean surface.
void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

// Presents the new render with whatever we've drawn onto it.
void Window::present() {
    SDL_GL_SwapWindow(platform->window);
}

// Changes the title displayed on the window bar.
void Window::setWindowTitle(std::string windowTitle) {
    this->windowTitle = windowTitle;
    if (platform->window)
        SDL_SetWindowTitle(platform->window, windowTitle.c_str());
}

// Changes the presentation mode (From the list of valid presentation modes)
void Window::setPresentationMode(window::PresentationMode presentationMode) {
    this->presentationMode = presentationMode;
    updateLogicalPresentation();
}

// Recalculates the variable information of the window, like its size or aspect ratio.
void Window::updateWindowData() {

    // Get current physical size of the window (In Pixels version is aware of DPR scale)
    SDL_GetWindowSizeInPixels(platform->window, &physicalSize.x, &physicalSize.y);

    // Get current aspect ratio
    aspectRatio = (float)physicalSize.x / (float)physicalSize.y;

    // Get current scale (Relative to the reference size)
    scale = Vector2(
        (float)physicalSize.x / referenceSize.x,
        (float)physicalSize.y / referenceSize.y
    );

    // Set OpenGL viewport to fill the window
    glViewport(0, 0, physicalSize.x, physicalSize.y);

    // Updates Logical Presentation (This process also updates logical size to match the new presentation)
    updateLogicalPresentation();
}

// Calculates the logical size depending on the presentation mode and updates presentation mode.
void Window::updateLogicalPresentation() {
    float refW = (float)referenceSize.x;
    float refH = (float)referenceSize.y;
    float winW = (float)physicalSize.x;
    float winH = (float)physicalSize.y;

    switch (presentationMode) {

    case window::PresentationMode::Free:
        // Viewport at position (0, 0) and the full size of the window.
        // Coordinates (logical size) are just the window size.
        glViewport(0, 0, physicalSize.x, physicalSize.y);
        logicalSize = physicalSize;
        break;

    case window::PresentationMode::Letterbox: {
        // Viewport centered, scales reference size to the smallest axis of the window.
        // Coordinates (logical size) based on reference size.
        float smallScale = std::min(scale.x, scale.y);
        int vpW = (int)(refW * smallScale);
        int vpH = (int)(refH * smallScale);
        int vpX = (int)((winW - vpW) / 2);
        int vpY = (int)((winH - vpH) / 2);
        glViewport(vpX, vpY, vpW, vpH);
        logicalSize = referenceSize;
        break;
    }

    case window::PresentationMode::Crop: {
        // Viewport centered, scales reference size to the biggest axis of the window.
        // Coordinates (logical size) based on reference size.
        float bigScale = std::max(winW / refW, winH / refH);
        int vpW = (int)(refW * bigScale);
        int vpH = (int)(refH * bigScale);
        int vpX = (int)((winW - vpW) / 2);
        int vpY = (int)((winH - vpH) / 2);
        glViewport(vpX, vpY, vpW, vpH);
        logicalSize = referenceSize;
        break;
    }

    case window::PresentationMode::Stretch:
        // Viewport at position (0, 0) and the full size of the window.
        // Coordinates (logical size) based on reference size.
        glViewport(0, 0, physicalSize.x, physicalSize.y);
        logicalSize = referenceSize;
        break;

    case window::PresentationMode::Expand: {
        // Viewport at position (0, 0) and the full size of the window.
        // Coordinates (logical size) are the reference size scaled to fit the screen.
        float smallScale = std::min(scale.x, scale.y);
        glViewport(0, 0, physicalSize.x, physicalSize.y);
        logicalSize = {(int)(winW / smallScale), (int)(winH / smallScale)};
        break;
    }

    case window::PresentationMode::ExpandHorizontal: {
        // Viewport at position (0, 0) and the full size of the window.
        // Coordinates (logical size) are the reference size scaled to fit the height of the screen.
        glViewport(0, 0, physicalSize.x, physicalSize.y);
        logicalSize = {(int)(winW / scale.y), (int)(winH / scale.y)};
        break;
    }

    case window::PresentationMode::ExpandVertical: {
        // Viewport at position (0, 0) and the full size of the window.
        // Coordinates (logical size) are the reference size scaled to fit the width of the screen.
        glViewport(0, 0, physicalSize.x, physicalSize.y);
        logicalSize = {(int)(winW / scale.x), (int)(winH / scale.x)};
        break;
    }

    default:
        glViewport(0, 0, physicalSize.x, physicalSize.y);
        logicalSize = physicalSize;
        break;
    }
}

// Destructor
Window::~Window() {
    if (platform->context) SDL_GL_DestroyContext(platform->context);
    if (platform->window) SDL_DestroyWindow(platform->window);
}
