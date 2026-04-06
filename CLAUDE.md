# Claude Instructions

When suggesting code changes, write the code in the chat rather than using edit tools. This is a beginner project and my goal is to learn, so I want to type as much myself as possible.

---

## Project Overview

**Pixelfall** is a custom 2D game engine built from scratch in C++17. Portfolio and learning project targeting Windows, with a long-term goal of a full editor experience. Currently in early development.

The sample game is a simple chess game — alternating turns, click a white piece then a black piece. No AI, no multiplayer.

**Tech Stack:**
- Language: C++17
- Build: CMake 4.3+
- Rendering: OpenGL 3.3 Core Profile (via GLAD)
- Window/Input: SDL3 (using `SDL_MAIN_USE_CALLBACKS` — no traditional `main()`)
- Font: SDL3_ttf
- External deps live in `pixelfall/external/`

---

## Architecture

```
pixelfall/                  # Engine — built as a static library
  engine/
    core/                   # Engine, Application, Window, Clock
    graphics/               # Painter, Shader, Mesh, Framebuffer, Color
    math/                   # Vector2, Vector3, Vector4 (+Int variants), numbers, tools
    config/                 # shader_config.h, window_config.h, application_config.h, painter_config.h
  platform/
    sdl/                    # SDL3 window, event callbacks
    opengl/                 # OpenGL implementations of Shader, Mesh, Framebuffer
  assets/shaders/opengl/    # GLSL shaders (geometry.vert/frag, screen.vert, fxaa_screen.frag, msaa_screen.frag)
  external/                 # glad, SDL3, SDL3_ttf
src/                        # Game code — MyGame inherits Application
```

**Key classes:**
- `Engine` — owns and orchestrates Window, Clock, Application; runs the main loop
- `Application` — virtual base with `onStart()`, `onUpdate()`, `onRender()` hooks; owns the Painter and shaders
- `Painter` — draw API; renders into a Framebuffer each frame and blits to screen with FXAA
- `Framebuffer` — FBO + color texture + fullscreen quad; used for the FXAA post-process pass
- `Shader` — loads/compiles GLSL from file, sets uniforms
- `Mesh` — VAO/VBO/EBO wrapper
- `Color` — RGBA (0.0–1.0 floats); supports construction from RGBA255, HSLA, HSVA, hex (3/4/6/8 digit)
- `Window` — platform-agnostic window; handles presentation modes and DPI-aware sizing
- `Clock` — deltaTime and FPS tracking

**Render flow each frame:**
1. `window.clear()` — clears default framebuffer
2. `painter.begin()` — recreates FBO if window resized, binds FBO, uploads ortho projection
3. `application.onRender()` — user draws shapes into FBO
4. `painter.end()` — blits FBO to screen through FXAA shader (or passthrough if MSAA granted)
5. `window.present()` — swap buffers

**Entry point:** Games use `PIXELFALL_APPLICATION(MyGame)` macro to register their class. Engine calls `createApplication()` which the macro implements.

**Platform abstraction:** Core classes (Window, Shader, Mesh, Framebuffer) forward-declare a private `PlatformComponents` struct in their header. The platform `.cpp` defines it with the actual API handles. This keeps platform code entirely out of engine headers.

**Anti-aliasing:** On startup, `Window::init()` requests 4x MSAA from the driver. If granted (`isMSAAEnabled()`), `Application::onSetup()` passes a plain passthrough shader to `Painter`. If not granted (common), FXAA is used instead.

---

## Conventions

See `docs/conventions.md` for full details. Key points:

- **Naming:** files/dirs camelCase, classes PascalCase, everything else camelCase
- **Shaders:** `name.vert.glsl` / `name.frag.glsl`
- **Config:** constants in `namespace domain { namespace defaults { ... } }`
- **Comments:** every function implementation has a comment on the line directly above it
- **Includes:** grouped and labelled — own header, Standard Library, engine subsystems, platform libs

---

## Current State

**Implemented:**
- Engine loop, SDL3 window, OpenGL 3.3 context
- Painter: `drawPolygon`, `drawTriangle`, `drawQuad`, `drawRectangle`, `drawCircle`, `drawArc`, `drawRegularPolygon`
- FXAA post-process AA (passthrough fallback when MSAA is granted by driver)
- Alpha blending (`GL_BLEND`)
- Color: RGBA/HSLA/HSVA/hex with full conversion, getters, and setters
- Vector2/Vector2Int with full operator overloads
- Presentation modes: Free, Letterbox, Crop, Stretch, Expand, ExpandHorizontal, ExpandVertical
- Clock with deltaTime, FPS tracking, optional frame cap

**Not yet implemented:**
- Input handling (SDL events not wired to Application)
- Outlined shape variants, `drawLine`, `drawLineStrip`
- Concave polygon support (ear clipping)
- Entity/component system
- Asset loading, audio
- Vector3/Vector4 operators (data-only structs currently)
- Editor
