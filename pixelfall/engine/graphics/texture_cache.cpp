// Implementation of textureCache.h

#include "pixelfall/engine/graphics/texture_cache.h"

// Loads a texture from the cache. If the texture is not in the cache, it creates it.
Texture& TextureCache::loadTexture(const std::string& path) {
    // [REVIEW] [EXPERIMENTAL]
    if (cache.count(path) == 0) {
        cache.emplace(std::piecewise_construct,
            std::forward_as_tuple(path),
            std::forward_as_tuple(path)
        );
    }
    return cache.at(path);
}

void TextureCache::clear() {
    cache.clear();
}