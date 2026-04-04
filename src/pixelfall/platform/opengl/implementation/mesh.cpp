// Implementation of mesh.h for OpenGL.

#include "pixelfall/engine/graphics/mesh.h"
// OpenGL
#include <glad/glad.h>

// Define components specific to the platform (SDL and OpenGL)
struct Mesh::PlatformComponents {
    // Create vertex array object (VAO) - Required when using OpenGL Core
    unsigned int VAO;
    // Create element buffer object (EBO) to store indices (Avoids sending duplicate vertices to draw overlapping triangles)
    unsigned int VBO;
    // Create vertex shader object
    unsigned int EBO;
};

// Constructor
Mesh::Mesh(
    std::vector<float> vertices,
    std::vector<unsigned int> indices
    ) :
    vertices(vertices),
    indices(indices),
    platform(std::make_unique<PlatformComponents>()) {

    // Set up VAO
    glGenVertexArrays(1, &platform->VAO);
    glBindVertexArray(platform->VAO);

    // Set up VBO
    glGenBuffers(1, &platform->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, platform->VBO);

    // Set up EBO
    glGenBuffers(1, &platform->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, platform->EBO);

    // Link Vertex Attributes (Tell the GPU how to interpret the vertex data we provided)
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    update();
};

void Mesh::update() {
    // Update VBO
    glBindVertexArray(platform->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, platform->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Update EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, platform->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void Mesh::draw() {
    glBindVertexArray(platform->VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh() = default;