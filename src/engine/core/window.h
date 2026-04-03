// Defines a window with all of the parameters and functions it should contain. 
// The actual implementation is platform specific.

#pragma once
#include <string>
#include "engine/math/vector2.h"
#include "engine/config/window_config.h"

class Window {
public:
    Window(
        std::string windowTitle = window::defaults::windowTitle.data(),
        Vector2Int physicalSize = window::defaults::physicalSize,
        Vector2Int minWindowSize = window::defaults::minWindowSize,
        float minAspectRatio = window::defaults::minAspectRatio,
        float maxAspectRatio = window::defaults::maxAspectRatio,
        float dprScale = window::defaults::dprScale,
        window::PresentationMode presentationMode = window::defaults::presentationMode
    );

    // Getters
    const Vector2Int& getPhysicalSize() const { return physicalSize; };
    float getDprScale() const { return dprScale; };
    const Vector2Int& getLogicalSize() const { return logicalSize; };
    float getAspectRatio() const { return aspectRatio; };
    const Vector2& getScale() const { return scale; };

    // Setters
    void setWindowTitle(std::string windowTitle);
    void setDprScale(float dprScale) { this->dprScale = dprScale; };
    void setPresentationMode(window::PresentationMode presentationMode);

    // Methods
    bool init();
    void clear();
    void present();
    void updateWindowData();
    void debug();

    // Destroyers
    ~Window();

private:
    // Variables
    std::string windowTitle;
    Vector2Int physicalSize;
    Vector2Int minWindowSize;
    float minAspectRatio;
    float maxAspectRatio;
    float dprScale;
    window::PresentationMode presentationMode;

    // Derived Variables
    Vector2Int logicalSize;
    float aspectRatio;
    Vector2 scale;

    // Platform Implementation (Each platform fills this struct with their necessary components)
    struct PlatformComponents;
    PlatformComponents* platformComponents = nullptr;

    // Methods
    void updateLogicalPresentation();
};