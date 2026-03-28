// Defines time information, such as fps and deltaTime.
#pragma once
#include <stdint.h>
#include "engine/config/engine_config.h"

class Clock {
public:
    Clock() = default;

    // Getters
    double getDeltaTime() const { return deltaTime; };
    double getFPS() const { return fps; };
    uint64_t getCurrentTime() const { return frameStartTime; };
    bool getLimitFrameRate() const { return limitFrameRate; };

    // Methods
    bool init();
    void update();
    void reset();
    void limitFPS();

private:
    uint64_t lastFrameStartTime;
    uint64_t frameStartTime;

    double deltaTime;
    double fps;

    // Settings
    bool limitFrameRate = engine::defaults::limitFrameRate;
    double fpsLimit = engine::defaults::fpsLimit;
};