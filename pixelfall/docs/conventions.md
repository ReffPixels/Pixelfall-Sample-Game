File Naming Conventions
- The default naming convention for files and folders is camelCase.
- C++ headers use .h
- .cpp and .h files use snake_case
- GLSL shader files use the format *name.vert.glsl* or *name.frag.glsl*

Code Naming Conventions
- Classes and Structs use PascalCase
- Namespaces use camelCase
- Functions use camelCase
- Identifiers use camelCase
- Constant Identifiers use camelCase

Code Principles
- Keep dependencies as separate as possible from the engine code (Encapsulation)
- Make the final syntax of tools by the game/application as simple as possible.
- Make an effort to keep all names descriptive and clear
- Add comments to complex cases to explain how they work

Code specific guidelines
- Smart pointers (std::unique_ptr) are preferred over raw owning pointers
- Members that are initialized late use std::optional<T> with .emplace() rather than pointers
- Alternative construction patterns use static factory methods named *fromName* (e.g. *Color::fromHexcode*)
- Math types (Vector2, Color, etc.) use public member variables directly — no getters/setters
- Helper functions used only within a .cpp file are declared static and forward-declared at the top of the file

File Structure Conventions
- Every file begins with a single-line comment describing its purpose
- Platform implementation files note which platform they serve (e.g. "// Implementation of window.h for SDL.")
- Includes are grouped and labelled by categories (Standard Library, Core, Graphics, Math, Configuration, etc)
- Class members are grouped under section comments: Getters, Setters, Methods, Variables, Destroyers
- The purpose of functions is described in the line just above it's implementation in the .cpp file

Config files
- Config constants live in a domain namespace (e.g. *painter_config.h* contains the painter:: namespace)
- Default values are in a nested namespace called defaults:: (e.g. *window::defaults::physicalSize*)
- String constants use constexpr std::string_view
- Aggregate types (Vector2Int, Color) use inline const since they cannot be constexpr

Platform Abstraction Conventions
- Platform-specific state is stored in a private struct PlatformComponents, forward-declared in the header and defined in the platform .cpp file
- The struct is owned by the core class as std::unique_ptr<PlatformComponents> platform
