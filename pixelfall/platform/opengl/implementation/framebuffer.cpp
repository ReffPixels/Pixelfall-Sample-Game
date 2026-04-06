// Implementation of framebuffer.h for OpenGL.

#include "pixelfall/engine/graphics/framebuffer.h"
// OpenGL
#include <glad/glad.h>

struct Framebuffer::PlatformComponents {
    unsigned int fbo = 0;
    unsigned int texture = 0;
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
};

Framebuffer::Framebuffer(Vector2Int size) : size(size), platform(std::make_unique<PlatformComponents>()) {

    // Create FBO and attach a color texture
    glGenFramebuffers(1, &platform->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, platform->fbo);

    glGenTextures(1, &platform->texture);
    glBindTexture(GL_TEXTURE_2D, platform->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, platform->texture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create fullscreen quad with NDC positions and UV coords for the FXAA blit pass
    float quadVertices[] = {
        // pos (xy)    // uv
        -1.0f,  1.0f,  0.0f, 1.0f,  // top-left
        -1.0f, -1.0f,  0.0f, 0.0f,  // bottom-left
         1.0f, -1.0f,  1.0f, 0.0f,  // bottom-right
        -1.0f,  1.0f,  0.0f, 1.0f,  // top-left
         1.0f, -1.0f,  1.0f, 0.0f,  // bottom-right
         1.0f,  1.0f,  1.0f, 1.0f   // top-right
    };

    glGenVertexArrays(1, &platform->quadVAO);
    glGenBuffers(1, &platform->quadVBO);
    glBindVertexArray(platform->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, platform->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    // location 0: position (xy)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // location 1: UV coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

// Binds the FBO — all subsequent draw calls go into this framebuffer
void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, platform->fbo);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Unbinds the FBO and sets the viewport to the full physical window for the blit pass
void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, size.x, size.y);
}

// Binds the FBO texture and draws the fullscreen quad
void Framebuffer::drawQuad() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, platform->texture);
    glBindVertexArray(platform->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

Framebuffer::~Framebuffer() {
    glDeleteVertexArrays(1, &platform->quadVAO);
    glDeleteBuffers(1, &platform->quadVBO);
    glDeleteTextures(1, &platform->texture);
    glDeleteFramebuffers(1, &platform->fbo);
}
