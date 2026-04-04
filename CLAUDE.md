# Claude Instructions

When suggesting code changes, write the code in the chat rather than using edit tools. This is a beginner project and my goal is to learn, so I want to type as much myself as possible.

---

## Project Overview

**Pixelfall** is a custom 2D game engine built from scratch in C++17. It is a portfolio and learning project targeting Windows, with the long-term goal of having a full editor experience similar to Unity/Unreal. Currently in early development (version 0.0).

**Tech Stack:**
- Language: C++17
- Build: CMake 3.4+
- Rendering: OpenGL 3.3 Core Profile (via GLAD)
- Window/Input: SDL3 (using SDL_MAIN_USE_CALLBACKS — no traditional main())
- Font: SDL3_ttf
- External deps live in `external/`

---

## Architecture

```
src/
  application/        # User-facing game code (MyGame inherits Application)
  pixelfall/
    engine/
      core/           # Engine, Application, Window, Clock
      graphics/       # Painter, Shader, Mesh, Color
      math/           # Vector2, Vector3, Vector4 (+Int variants), constants
      config/         # engine_config.h, window_config.h
    platform/
      sdl/            # SDL3 window, event callbacks
      opengl/         # OpenGL shader, mesh implementations
assets/shaders/       # GLSL vertex and fragment shaders
```

**Key classes:**
- `Engine` — main loop orchestrator; owns Window, Clock, Application
- `Application` — virtual base class with `onStart()`, `onUpdate()`, `onRender()` hooks
- `MyGame` — user's game class (in `src/application/`)
- `Painter` — high-level draw API (currently only `drawTriangle()`)
- `Shader` — loads/compiles GLSL from file, sets uniforms
- `Mesh` — VAO/VBO/EBO wrapper
- `Color` — RGBA (0.0–1.0 floats); factory methods: `fromRGBA255`, `fromHSLA`, `fromHexcode`, etc.
- `Clock` — deltaTime and FPS tracking
- `Window` — platform-agnostic window with configurable presentation modes (Letterbox, Crop, Stretch, etc.)

**Platform abstraction pattern:** Core classes (Window, Shader, Mesh) hold opaque `PlatformComponents` pointers. Implementations live in `platform/`.

---

## Conventions

- **Files/dirs:** camelCase
- **Classes/Structs:** PascalCase
- **Methods/variables:** camelCase
- **Namespaces:** camelCase (e.g., `engine::`, `window::`)
- Headers: `.h`, implementations: `.cpp`
- Smart pointers (`std::unique_ptr`) preferred; `Application` created via `Application::create()` factory
- Config constants grouped in namespaces with a nested `defaults::` namespace

See `docs/conventions.md` for full details.

---

## Current State

**Implemented:**
- Engine init/update/shutdown loop
- SDL3 window + OpenGL 3.3 context
- Shader loading and uniform setters
- Painter with `drawTriangle()`
- Mesh (VAO/VBO/EBO)
- Color with full RGBA/HSLA/HSVA/Hex conversion
- Vector2/Vector2Int with full operator overloads
- Clock with FPS tracking and optional frame cap
- Configurable window presentation modes

**Not yet implemented:**
- Input handling (SDL events received but not wired to Application)
- More Painter primitives (rect, circle, line, etc.)
- Entity/component system
- Asset loading system
- Audio
- Vector3/Vector4 full operator support
- Editor
