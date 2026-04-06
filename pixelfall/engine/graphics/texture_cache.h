// Caches textures by path to avoid reloading them from disk.

#pragma once
#include <map>
#include <string>
#include "pixelfall/engine/graphics/texture.h"

class TextureCache {
public:
    // Methods
    Texture& loadTexture(const std::string& path);
    void clear();

private:
    std::map<std::string, Texture> cache;
};