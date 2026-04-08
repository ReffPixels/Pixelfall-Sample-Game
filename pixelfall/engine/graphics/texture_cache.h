// Caches textures by path to avoid reloading them from disk.

#pragma once
#include <map>
#include <string>
#include "pixelfall/engine/graphics/texture.h"

class TextureCache {
public:
    // Constructor
    TextureCache(const std::filesystem::path& projectPath)
        : projectPath(projectPath) {
    }

    // Methods
    Texture& loadTexture(const std::filesystem::path& path);
    void clear();

private:
    std::filesystem::path projectPath;
    std::map<std::filesystem::path, Texture> cache;
};