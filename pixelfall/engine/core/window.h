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
    const Vector2& getScale() const { return scale; };

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

    // Destroyers
    ~Window();

private:
    // Variables
    std::string windowTitle;
    Vector2Int physicalSize;
    Vector2Int referenceSize;
    Vector2Int minWindowSize;
    float minAspectRatio;
    float maxAspectRatio;
    float dprScale;
    window::PresentationMode presentationMode;
    Color clearColor;

    // Derived Variables
    Vector2Int logicalSize;
    float aspectRatio;
    Vector2 scale;

    // Settings
    bool msaaGranted = false;

    // Platform Implementation (Each platform fills this struct with their necessary components)
    struct PlatformComponents;
    std::unique_ptr<PlatformComponents> platform;

    // Methods
    void updateLogicalPresentation();
};