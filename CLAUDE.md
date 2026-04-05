# Claude Instructions

When suggesting code changes, write the code in the chat rather than using edit tools. This is a beginner project and my goal is to learn, so I want to type as much myself as possible.

---

## Project Overview

**Pixelfall** is a custom 2D game engine built from scratch in C++17. It is a portfolio and learning project targeting Windows, with the long-term goal of having a full editor experience similar to Unity/Unreal. Currently in early development (version 0.0).

The sample game in this repo is a simple chess game — alternating turns, click a white piece then a black piece. No AI, no multiplayer.

**Tech Stack:**
- Language: C++17
- Build: CMake 4.3+
- Rendering: OpenGL 3.3 Core Profile (via GLAD)
- Window/Input: SDL3 (using SDL_MAIN_USE_CALLBACKS — no traditional main())
- Font: SDL3_ttf
- External deps live in `pixelfall/external/`

---

## Architecture

```
pixelfall/                  # Engine — built as a static library
  engine/
    core/                   # Engine, Application, Window, Clock
    graphics/               # Painter, Shader, Mesh, Color
    math/                   # Vector2, Vector3, Vector4 (+Int variants), constants
    config/                 # engine_config.h, window_config.h, application_config.h, shader_config.h
  platform/
    sdl/                    # SDL3 window, event callbacks
    opengl/                 # OpenGL shader, mesh implementations
  assets/shaders/opengl/    # GLSL vertex and fragment shaders
  external/                 # glad, SDL3, SDL3_ttf
src/                        # User-facing game code (MyGame inherits Application)
```

**Key classes:**
- `Engine` — main loop orchestrator; owns Window, Clock, Application
- `Application` — virtual base class with `onStart()`, `onUpdate()`, `onRender()` hooks
- `MyGame` — user's game class (in `src/`)
- `Painter` — high-level draw API (currently only `drawTriangle()`)
- `Shader` — loads/compiles GLSL from file, sets uniforms
- `Mesh` — VAO/VBO/EBO wrapper
- `Color` — RGBA (0.0–1.0 floats); factory methods: `fromRGBA255`, `fromHSLA`, `fromHexcode`, etc.
- `Clock` — deltaTime and FPS tracking
- `Window` — platform-agnostic window with configurable presentation modes (Letterbox, Crop, Stretch, etc.)

**Entry point pattern:** Games use the `PIXELFALL_APPLICATION(GameClass)` macro (defined in `application.h`) to register their game class. The engine calls `createApplication()` during init, which the macro implements.

**Platform abstraction pattern:** Core classes (Window, Shader, Mesh) hold opaque `PlatformComponents` pointers. Implementations live in `platform/`.

**CMake structure:** `pixelfall/` is a static library with its own `CMakeLists.txt`. The root `CMakeLists.txt` defines the game executable and consumes pixelfall via `add_subdirectory`. Games call `pixelfall_setup_runtime(target)` to copy engine assets and DLLs post-build. External dependencies (glad, SDL3) live in `pixelfall/external/` and are fully owned by the engine.

---

## Conventions

- **Files/dirs:** camelCase
- **Classes/Structs:** PascalCase
- **Methods/variables:** camelCase
- **Namespaces:** camelCase (e.g., `engine::`, `window::`)
- Headers: `.h`, implementations: `.cpp`
- Smart pointers (`std::unique_ptr`) preferred; game registers entry point via `PIXELFALL_APPLICATION(GameClass)` macro
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
- Configurable window presentation modes (Free, Letterbox, Crop, Stretch, Expand, ExpandHorizontal, ExpandVertical)
- Application metadata (title, version, identifier) via virtual getters with defaults from `application_config.h`
- `PIXELFALL_APPLICATION(GameClass)` entry point macro
- pixelfall as a self-contained static library with its own CMakeLists and external dependencies

**Not yet implemented:**
- Input handling (SDL events received but not wired to Application)
- More Painter primitives (rect, circle, line, etc.)
- Entity/component system
- Asset loading system
- Audio
- Vector3/Vector4 operator support (currently data-only structs)
- Editor
- `#ifdef` platform guards in `shader_config.h` (noted as TODO)
