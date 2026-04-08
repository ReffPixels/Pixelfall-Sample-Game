// Implementation of textureCache.h

#include "pixelfall/engine/graphics/texture_cache.h"

// Loads a texture from the cache. If the texture is not in the cache, it creates it.
Texture& TextureCache::loadTexture(const std::filesystem::path& path) {
    // [REVIEW] [EXPERIMENTAL]
    std::filesystem::path fullPath{projectPath / path};

    if (cache.count(fullPath) == 0) {
        cache.emplace(std::piecewise_construct,
            std::forward_as_tuple(fullPath),
            std::forward_as_tuple(fullPath)
        );
    }
    return cache.at(fullPath);
}

void TextureCache::clear() {
    cache.clear();
}