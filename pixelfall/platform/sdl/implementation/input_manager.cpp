// Implementation of input.h for SDL.

#include "pixelfall/engine/input/input_manager.h"
// SDL
#include <SDL3/SDL.h>
// Input
#include "pixelfall/platform/sdl/implementation/sdl_keycodes.h"

// Called when an input event happens.
void InputManager::onEvent(const void* platformEvent) {
    // [TODO] In the future, make an engine event system instead of just copying SDL's pointer
    const SDL_Event* event = static_cast<const SDL_Event*>(platformEvent);

    // Check what kind of event was detected
    switch (event->type) {
    case SDL_EVENT_KEY_DOWN: {
        // Make sure this is the first frame of the key being pressed. 
        // This is only necessary for KEY_DOWN in SDL
        if (event->key.repeat) break;

        // Map SDL Key Code to Pixelfall Key Code
        KeyCode keyCode = sdlToKeyCode(event->key.key);

        // Check if Key type is supported
        if (keyCode != KeyCode::Unknown) {
            keysDown[keyCode] = true;
            keysPressed[keyCode] = true;
        }
        break;
    }
    case SDL_EVENT_KEY_UP: {
        // Map SDL Key Code to Pixelfall Key Code
        KeyCode keyCode = sdlToKeyCode(event->key.key);

        // Check if Key type is supported
        if (keyCode != KeyCode::Unknown) {
            keysDown[keyCode] = false;
            keysReleased[keyCode] = true;
        }
        break;
    }
    case SDL_EVENT_MOUSE_MOTION: {
        // Update mouse position every time the mouse moves. [RESEARCH: Is this overkill?]
        mousePhysicalPosition = Vector2(event->motion.x, event->motion.y);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        // Map SDL Button Code to Pixelfall Button Code
        MouseButton buttonCode{sdlToMouseButton(event->button.button)};

        // Check if this Button type is supported
        if (buttonCode != MouseButton::Unknown) {
            mouseButtonsDown[buttonCode] = true;
            mouseButtonsPressed[buttonCode] = true;
        }
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP: {
        // Map SDL Button Code to Pixelfall Button Code
        MouseButton buttonCode{sdlToMouseButton(event->button.button)};

        // Check if Button Code is supported
        if (buttonCode != MouseButton::Unknown) {
            mouseButtonsDown[buttonCode] = false;
            mouseButtonsReleased[buttonCode] = true;
        }
        break;
    }
    }
}

// Check if a key is down 
// True for every frame where the key is being held down, including the first frame.
bool InputManager::isKeyDown(KeyCode key) const {
    auto searchResult = keysDown.find(key);
    // Check if the search result was succesful (Did not return end) and if the key's value (->second) is true
    return (searchResult != keysDown.end() && searchResult->second);
}

// Check if a key was pressed
// True only during the first frame.
bool InputManager::isKeyPressed(KeyCode key) const {
    auto searchResult = keysPressed.find(key);
    return (searchResult != keysPressed.end() && searchResult->second);
}

// Check if a key was released
// True only during the first non-pressed frame.
bool InputManager::isKeyReleased(KeyCode key) const {
    auto searchResult = keysReleased.find(key);
    return (searchResult != keysReleased.end() && searchResult->second);
}

// Check if a mouse button is down 
// True for every frame where the mouse button is being held down, including the first frame.
bool InputManager::isMouseButtonDown(MouseButton button) const {
    auto searchResult = mouseButtonsDown.find(button);
    return (searchResult != mouseButtonsDown.end() && searchResult->second);
}

// Check if a mouse button was pressed
// True only during the first frame.
bool InputManager::isMouseButtonPressed(MouseButton button) const {
    auto searchResult = mouseButtonsPressed.find(button);
    return (searchResult != mouseButtonsPressed.end() && searchResult->second);
}

// Check if a mouse button was released
// True only during the first non-pressed frame.
bool InputManager::isMouseButtonReleased(MouseButton button) const {
    auto searchResult = mouseButtonsReleased.find(button);
    return (searchResult != mouseButtonsReleased.end() && searchResult->second);
}

// Called at the end of each frame, clears frame dependant maps.
void InputManager::onEndOfFrame() {
    keysPressed.clear();
    keysReleased.clear();
    mouseButtonsPressed.clear();
    mouseButtonsReleased.clear();
}

// [TODO OPTIMIZATION] Currently keys are inserted into their maps as they are detected, which is a little inneficient.
// A better method would be to fill all maps with false in a constructor so they don't change size, 
// and reset them to false instead of using.clear. Would also mean replacing [value] with .at(value)