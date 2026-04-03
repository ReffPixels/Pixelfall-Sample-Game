// Defines the Loop of this application.

#include "application/my_game.h"
// Output to console
#include <iostream>
// Shaders
#include <glad/glad.h>

std::unique_ptr<Application> Application::create() {
    return std::make_unique<MyGame>();
}

// Create vertex array object (VAO) - Required when using OpenGL Core
unsigned int VAO;
// Create element buffer object (EBO) to store indices (Avoids sending duplicate vertices to draw overlapping triangles)
unsigned int VBO;
// Create vertex shader object
unsigned int EBO;

bool MyGame::onStart(std::filesystem::path projectPath) {
    // Debug
    std::cout << "START" << std::endl;

    basicShader.emplace(
        (projectPath / "assets/shaders/vertex_shader.glsl"),
        (projectPath / "assets/shaders/fragment_shader.glsl")
    );

    // Define vertices in Normalized Device Coordinates (NDC) 
    //Keep Z axis to 0 for 2D (Equal depth)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        1.0f, 0.0f, 0.0f,

        -0.25f, 0.5f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, -0.5f, 0.0f,
        0.0f, 0.0f, 1.0f

    };
    unsigned int indices[] = {
        0, 1, 2
    };

    // Set up VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set up VBO - We first need to store the space in memory by declaring a variable, 
    // then create the buffer and assign it to that space,
    // and finaly we bind our VBO to the correct buffer type (GL ARRAY BUFFER for Vertex buffers)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy user defined data into the currently bound buffer. 
    // Set GPU mode to STATIC DRAW since the triangle won't change often.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Link Vertex Attributes (Tell the GPU how to interpret the vertex data we provided)
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return true;
}

void MyGame::onUpdate() {
}

void MyGame::onRender() {
    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Use shader program
    if (basicShader) {
        basicShader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    else {
        std::cout << "ERROR: Shader uninitialized (Has onRender been called before onStart?)" << std::endl;
    }
}
