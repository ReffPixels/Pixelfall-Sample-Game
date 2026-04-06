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
    std::vector<unsigned int> indices,
    std::vector<VertexAttribute> attributes
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

    // Link Vertex Attributes
    // Calculate the stride (All attribute components added together)
    int stride{0};
    for (auto& attr : attributes) stride += attr.components;

    // Loop through every attribute and link it with the correct information
    int offset{0};
    for (int i = 0; i < (int)attributes.size(); i++) {
        glVertexAttribPointer(
            i, attributes[i].components,
            GL_FLOAT,
            GL_FALSE,
            stride * sizeof(float),
            (void*)(offset * sizeof(float))
        );
        glEnableVertexAttribArray(i);
        // Record offset for next attribute
        offset += attributes[i].components;
    }

    update();
};

// Calculates mesh components. 
// Called by the constructor when instantiating the mesh, can be called again externally if needed.
void Mesh::update() {
    // Update VBO
    glBindVertexArray(platform->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, platform->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Update EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, platform->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

// Displays the mesh on the screen by drawing each of it's triangles.
void Mesh::draw() {
    glBindVertexArray(platform->VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Destroyer
Mesh::~Mesh() = default;