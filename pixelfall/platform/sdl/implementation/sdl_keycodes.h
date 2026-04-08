#include "pixelfall/engine/input/input_manager.h"
// SDL
#include <SDL3/SDL.h>

// Translate SDL mouse button types into a Pixelfall MouseButton
static MouseButton sdlToMouseButton(Uint8 code) {
    switch (code) {
    case SDL_BUTTON_LEFT: return MouseButton::Left;
    case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
    case SDL_BUTTON_RIGHT: return MouseButton::Right;
    case SDL_BUTTON_X1: return MouseButton::X1;
    case SDL_BUTTON_X2: return MouseButton::X2;
    default: return MouseButton::Unknown;
    }
}

// Translate SDL KeyCode into a Pixelfall KeyCode
static KeyCode sdlToKeyCode(SDL_Keycode code) {
    switch (code) {
    // Letters
    case SDLK_A: return KeyCode::A;
    case SDLK_B: return KeyCode::B;
    case SDLK_C: return KeyCode::C;
    case SDLK_D: return KeyCode::D;
    case SDLK_E: return KeyCode::E;
    case SDLK_F: return KeyCode::F;
    case SDLK_G: return KeyCode::G;
    case SDLK_H: return KeyCode::H;
    case SDLK_I: return KeyCode::I;
    case SDLK_J: return KeyCode::J;
    case SDLK_K: return KeyCode::K;
    case SDLK_L: return KeyCode::L;
    case SDLK_M: return KeyCode::M;
    case SDLK_N: return KeyCode::N;
    case SDLK_O: return KeyCode::O;
    case SDLK_P: return KeyCode::P;
    case SDLK_Q: return KeyCode::Q;
    case SDLK_R: return KeyCode::R;
    case SDLK_S: return KeyCode::S;
    case SDLK_T: return KeyCode::T;
    case SDLK_U: return KeyCode::U;
    case SDLK_V: return KeyCode::V;
    case SDLK_W: return KeyCode::W;
    case SDLK_X: return KeyCode::X;
    case SDLK_Y: return KeyCode::Y;
    case SDLK_Z: return KeyCode::Z;
    // Numbers
    case SDLK_0: return KeyCode::N0;
    case SDLK_1: return KeyCode::N1;
    case SDLK_2: return KeyCode::N2;
    case SDLK_3: return KeyCode::N3;
    case SDLK_4: return KeyCode::N4;
    case SDLK_5: return KeyCode::N5;
    case SDLK_6: return KeyCode::N6;
    case SDLK_7: return KeyCode::N7;
    case SDLK_8: return KeyCode::N8;
    case SDLK_9: return KeyCode::N9;
    // Arrow Keys
    case SDLK_UP:    return KeyCode::Up;
    case SDLK_DOWN:  return KeyCode::Down;
    case SDLK_LEFT:  return KeyCode::Left;
    case SDLK_RIGHT: return KeyCode::Right;
    // Common Keys
    case SDLK_SPACE:     return KeyCode::Space;
    case SDLK_RETURN:    return KeyCode::Enter;
    case SDLK_ESCAPE:    return KeyCode::Escape;
    case SDLK_TAB:       return KeyCode::Tab;
    case SDLK_BACKSPACE: return KeyCode::Backspace;
    case SDLK_DELETE:    return KeyCode::Delete;
    // Modifiers
    case SDLK_LSHIFT: case SDLK_RSHIFT:  return KeyCode::Shift;
    case SDLK_LCTRL:  case SDLK_RCTRL:   return KeyCode::Control;
    case SDLK_LALT:   case SDLK_RALT:    return KeyCode::Alt;
    // Function Keys
    case SDLK_F1:  return KeyCode::F1;
    case SDLK_F2:  return KeyCode::F2;
    case SDLK_F3:  return KeyCode::F3;
    case SDLK_F4:  return KeyCode::F4;
    case SDLK_F5:  return KeyCode::F5;
    case SDLK_F6:  return KeyCode::F6;
    case SDLK_F7:  return KeyCode::F7;
    case SDLK_F8:  return KeyCode::F8;
    case SDLK_F9:  return KeyCode::F9;
    case SDLK_F10: return KeyCode::F10;
    case SDLK_F11: return KeyCode::F11;
    case SDLK_F12: return KeyCode::F12;
    // Other
    default: return KeyCode::Unknown;
    }
}