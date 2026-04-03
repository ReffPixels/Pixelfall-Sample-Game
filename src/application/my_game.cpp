// Defines the Loop of this application.

#include "application/my_game.h"
#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <SDL3/SDL.h>

std::unique_ptr<Application> Application::create() {
    return std::make_unique<MyGame>();
}

// Create vertex array object (VAO) - Required when using OpenGL Core
unsigned int VAO;
// Create element buffer object (EBO) to store indices (Avoids sending duplicate vertices to draw overlapping triangles)
unsigned int VBO;
// Create vertex shader object
unsigned int EBO;
// Create vertex buffer object (VBO) to send our vertices to the GPU in batches.
unsigned int vertexShader;
// Create shader program
unsigned int shaderProgram;

// Load shaders from files
static std::string loadShaderSource(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERROR::SHADER::FILE_NOT_FOUND: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool MyGame::onStart(std::filesystem::path projectPath) {

    // Load shaders from files (Store as string but use const char* for OpenGL)
    std::string vertSrc = loadShaderSource(projectPath / "assets/shaders/vertex_shader.glsl");
    std::string fragSrc = loadShaderSource(projectPath / "assets/shaders/fragment_shader.glsl");
    const char* vertexShaderSource = vertSrc.c_str();
    const char* fragmentShaderSource = fragSrc.c_str();

    std::cout << "START" << std::endl;

    // Define vertices in Normalized Device Coordinates (NDC) 
    //Keep Z axis to 0 for 2D (Equal depth)
    float vertices[] = {
        // Triangle 1
        -0.5f,  -0.5f, 0.0f,
        -0.25f,  0.5f, 0.0f,
        // Shared Corner
        0.0f,  -0.5f, 0.0f,
        // Triangle 2
        0.5f, -0.5f, 0.0f,
        0.25f, 0.5f, 0.0f,
    };
    unsigned int indices[] = {
        0, 1, 2, // Top right triangle
        2, 3, 4 // Bottom left triangle
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

    // Set up vertex shader object
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach shader source code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if compilation was succesful
    int vertexShaderSuccess;
    char vertexShaderinfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess);
    if (!vertexShaderSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderinfoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexShaderinfoLog << std::endl;
    }

    // Create fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach shader source code
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check if compilation was succesful
    int fragmentShaderSuccess;
    char fragmentShaderinfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSuccess);
    if (!fragmentShaderSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderinfoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentShaderinfoLog << std::endl;
    }

    // Set up shader program
    shaderProgram = glCreateProgram();

    // Link shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if linking was succesful
    int shaderProgramSuccess;
    char shaderProgramInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramSuccess);
    if (!shaderProgramSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderProgramInfoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << shaderProgramInfoLog << std::endl;
    }

    // Delete shader objects (We don't need them anymore)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Link Vertex Attributes (Tell the GPU how to interpret the vertex data we provided)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return true;
}

void MyGame::onUpdate() {
}

void MyGame::onRender() {
    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float timeValue = SDL_GetTicks() / 1000.0f;
    float colourOverTime = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColourLocation = glGetUniformLocation(shaderProgram, "customColor");

    // Use shader program
    glUseProgram(shaderProgram);
    glUniform4f(vertexColourLocation, 0.0f, colourOverTime, 0.0f, 1.0f);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
