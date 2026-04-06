// Implementation of texture.h for OpenGL.

#include "pixelfall/engine/graphics/texture.h"
// Standard Library
#include <iostream>
// OpenGL
#include <glad/glad.h>
// STB Image
#define STB_IMAGE_IMPLEMENTATION
#include "pixelfall/external/plugins/stb_image.h"

// Define components specific to the platform (SDL and OpenGL)
struct Texture::PlatformComponents {
    unsigned int textureID;
};

// Constructor
Texture::Texture(const std::filesystem::path& imagePath) :
    size{0, 0},
    platform(std::make_unique<PlatformComponents>()) {

    // Set up for loading image
    std::filesystem::path adjustedImagePath = imagePath;
    stbi_set_flip_vertically_on_load(true);
    int colorChannels; // Number of color channels

    // Load Image data from path
    unsigned char* imageData = stbi_load(
        adjustedImagePath.make_preferred().string().c_str(),
        &size.x, &size.y,
        &colorChannels, 4
    );

    if (imageData) {
        // Create and Bind Texture
        glGenTextures(1, &platform->textureID);
        glBindTexture(GL_TEXTURE_2D, platform->textureID);

        // Set a border color (To make it obvious when the image is outside the valid space)
        float outOfBoundsColor[] = {1.0f, 0.0f, 1.0f, 1.0f};

        // Set Texture Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, outOfBoundsColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // Filter mode when scaled down
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   // Filter mode when scaled up

        // Generate Texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    }
    else {
        std::cout << "ERROR: Failed to load texture: " << stbi_failure_reason() << std::endl;
    }

    // Free the image memory
    stbi_image_free(imageData);
};

// Binds the texture again so it can be used by draw calls.
void Texture::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, platform->textureID);
}

// Destroyer
Texture::~Texture() {
    glDeleteTextures(1, &platform->textureID);
};