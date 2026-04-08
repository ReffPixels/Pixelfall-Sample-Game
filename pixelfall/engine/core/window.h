// Defines a window with all of the parameters and functions it should contain. 
// The actual implementation is platform specific.

#pragma once
// Standard Library
#include <string>
#include <memory>
// Variable Types
#include "pixelfall/engine/math/vector2.h"
// Configuration
#include "pixelfall/engine/config/window_config.h"

class Window {
public:
    Window(
        std::string windowTitle = window::defaults::windowTitle.data(),
        Vector2Int physicalSize = window::defaults::physicalSize,
        Vector2Int referenceSize = window::defaults::referenceSize,
        Vector2Int minWindowSize = window::defaults::minWindowSize,
        float minAspectRatio = window::defaults::minAspectRatio,
        float maxAspectRatio = window::defaults::maxAspectRatio,
        float dprScale = window::defaults::dprScale,
        window::PresentationMode presentationMode = window::defaults::presentationMode,
        Color clearColor = window::defaults::clearColor
        );

    // Getters
    const Vector2Int& getPhysicalSize() const { return physicalSize; };
    bool isMSAAEnabled() const { return msaaGranted; };
    const Vector2Int& getReferenceSize() const { return referenceSize; };
    float getDprScale() const { return dprScale; };
    const Vector2Int& getLogicalSize() const { return logicalSize; };
    float getAspectRatio() const { return aspectRatio; };
    const Vector2& getReferenceScale() const { return referenceScale; };
    const Vector2& getViewportScale() const { return viewportScale; };
    const Vector2& getViewportOffset() const { return viewportOffset; };

    // Setters
    void setWindowTitle(std::string windowTitle);
    void setWReferenceSize(Vector2Int referenceSize) { this->referenceSize = referenceSize; };
    void setDprScale(float dprScale) { this->dprScale = dprScale; };
    void setPresentationMode(window::PresentationMode presentationMode);

    // Methods
    bool init();
    void clear();
    void present();
    void updateWindowData();
    Vector2 physicalToLogical(Vector2 position) const;

    // Destroyers
    ~Window();

private:
    // Title shown at the top bar of the window in windowed mode.
    std::string windowTitle;
    // Raw size of the window in pixels
    Vector2Int physicalSize;
    // Size used as a reference for coordinate space (Set in window config)
    Vector2Int referenceSize;
    // Settings
    Vector2Int minWindowSize;
    float minAspectRatio;
    float maxAspectRatio;
    // [TODO] DPR scale does not currently do anything. It should match the display's dprScale.
    float dprScale;
    // Rules for resizing/scaling the window
    window::PresentationMode presentationMode;
    // Colour used to clear the window - Determines colour of black bars
    Color clearColor;

    // Size in usable coordinates (Logical units) - Based on referenceSize, varies by presentation mode
    Vector2Int logicalSize{};
    float aspectRatio = 1.0f;
    // Scale of the window based on the referenceSize (1.0 is equal, 2.0 means the window is twice as big in pixels)
    Vector2 referenceScale {1.0f, 1.0f};
    // Distance from the black bars (Only matters for Letterbox and Crop presentations)
    Vector2 viewportOffset {Vector2::Zero};
    // Scale of the viewport (Often the same as referenceScale, but not in the case of expand modes where everything is sacled by the smallest axis)
    Vector2 viewportScale {1.0f, 1.0f};

    // Other Settings
    bool msaaGranted = false;

    // Platform Implementation (Each platform fills this struct with their necessary components)
    struct PlatformComponents;
    std::unique_ptr<PlatformComponents> platform;

    // Methods
    void updateLogicalPresentation();
};

// [TODO] Store viewportSize? (Actual pixels visible, specifically for crop and letterbox)