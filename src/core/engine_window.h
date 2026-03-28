#pragma once
#include <string>
#include "math/vector2.h"
#include "config/window_config.h"

class EngineWindow {
public:
    EngineWindow(
        std::string windowTitle = window::default::windowTitle.data(),
        Vector2Int physicalSize = window::default::physicalSize,
        Vector2Int minWindowSize = window::default::minWindowSize,
        float minAspectRatio = window::default::minAspectRatio,
        float maxAspectRatio = window::default::maxAspectRatio,
        float dprScale = window::default::dprScale,
        window::PresentationMode presentationMode = window::default::presentationMode
    );

    // Getters
    const Vector2Int &getPhysicalSize() const { return physicalSize; };
    float getDprScale() const { return dprScale; };
    const Vector2Int &getLogicalSize() const { return logicalSize; };
    float getAspectRatio() const { return aspectRatio; };
    const Vector2 &getScale() const { return scale; };

    // Setters
    void setWindowTitle(std::string windowTitle);
    void setDprScale(float dprScale) { this->dprScale = dprScale; };
    void setPresentationMode(window::PresentationMode presentationMode) {
        this->presentationMode = presentationMode;
    };

    // Methods
    bool init();
    void clear();
    void present();
    void updateWindowData();
    void debug();

    // Destroyers
    ~EngineWindow();

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
    PlatformComponents *platformComponents = nullptr;

    // Methods
    void updateLogicalSize();
};