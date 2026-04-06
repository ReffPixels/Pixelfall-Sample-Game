// Defines a basic texture
// The actual implementation is platform specific.

#pragma once
// Standard Library
#include <filesystem>
#include <memory>
// Variable Types
#include "pixelfall/engine/math/vector2.h"

class Texture {
public:
    Texture(const std::filesystem::path& imagePath);

    // Getters
    Vector2Int getSize() const { return size; };

    // Methods
    void bind();

    // Destroyers
    ~Texture();
private:
    // Variables
    Vector2Int size;

    // Platform Implementation (Each platform fills this struct with their necessary components)
    struct PlatformComponents;
    std::unique_ptr<PlatformComponents> platform;
};