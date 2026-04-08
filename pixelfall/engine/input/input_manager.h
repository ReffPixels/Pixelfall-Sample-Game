// Defines the Input class for Keyboard and Mouse detection

#pragma once
// Standard Library
#include <map>
// Math
#include "pixelfall/engine/math/vector2.h"
// Input
#include "pixelfall/engine/input/keycodes.h"

class InputManager {
public:
    // Keyboard State
    bool isKeyDown(KeyCode key) const;
    bool isKeyUp(KeyCode key) const;
    bool isKeyPressed(KeyCode key) const;
    bool isKeyReleased(KeyCode key) const;

    // Mouse State
    bool isMouseButtonDown(MouseButton button) const;
    bool isMouseButtonUp(MouseButton button) const;
    bool isMouseButtonPressed(MouseButton button) const;
    bool isMouseButtonReleased(MouseButton button) const;
    Vector2 getMousePhysicalPosition() { return mousePhysicalPosition; };

    // Methods
    void onEvent(const void* platformEvent);
    void onEndOfFrame();
private:
    // Keyboard State
    std::map<KeyCode, bool> keysDown;
    std::map<KeyCode, bool> keysPressed;
    std::map<KeyCode, bool> keysReleased;

    // Mouse state
    std::map<MouseButton, bool> mouseButtonsDown{};
    std::map<MouseButton, bool> mouseButtonsPressed{};
    std::map<MouseButton, bool> mouseButtonsReleased{};
    Vector2 mousePhysicalPosition{Vector2::Zero};
};