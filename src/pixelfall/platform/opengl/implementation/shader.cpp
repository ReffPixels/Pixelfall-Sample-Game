#include "pixelfall/engine/rendering/shader.h"
// OpenGL
#include <glad/glad.h>
// File Management
#include <fstream>
#include <sstream>
// Printing to Console
#include <iostream>

// Define components specific to the platform (OpenGL)
struct Shader::PlatformComponents {
    const char* vertexShaderCodeOpenGL;
    const char* fragmentShaderCodeOpenGL;
};

// OpenGL only functions (Forward declarations, not in header file)
static void configureShader(unsigned int shader, const char* shaderCode);
static void configureShaderProgram(unsigned int shaderID, unsigned int vertexShader, unsigned int fragmentShader);

// Constructor - Creates the shader and fully sets it up  so it's ready to use.
Shader::Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath) {
    // Create platform components
    platformComponents = new PlatformComponents();

    // Get shader code (OpenGL needs this in const char* format)
    vertexShaderCode = getShaderCode(vertexShaderPath);
    fragmentShaderCode = getShaderCode(fragmentShaderPath);
    platformComponents->vertexShaderCodeOpenGL = vertexShaderCode.c_str();
    platformComponents->fragmentShaderCodeOpenGL = fragmentShaderCode.c_str();

    // Create Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    configureShader(vertexShader, platformComponents->vertexShaderCodeOpenGL);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    configureShader(fragmentShader, platformComponents->fragmentShaderCodeOpenGL);

    // Create Shader Program
    shaderID = glCreateProgram();
    configureShaderProgram(shaderID, vertexShader, fragmentShader);

    // Delete shaders now that they are already linked to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
};

// Uses the shader program
void Shader::use() {
    glUseProgram(shaderID);
}

void Shader::addUniformBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}

void Shader::addUniformInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::addUniformFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

// Load shaders from files
std::string Shader::getShaderCode(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERROR: Shader file not found: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// Configures individual shader (Vertex or Fragment)
void configureShader(unsigned int shader, const char* shaderCode) {
    // Attach Code
    glShaderSource(shader, 1, &shaderCode, NULL);

    // Compile
    glCompileShader(shader);

    // Check if compilation was succesful
    int compilationSuccess{0};
    char compilationInfoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
    if (!compilationSuccess) {
        glGetShaderInfoLog(shader, 512, NULL, compilationInfoLog);
        std::cout << "ERROR: Shader compilation failed: " << compilationInfoLog << std::endl;
    }
}

// Configures shader program (Links shaders)
void configureShaderProgram(unsigned int shaderID, unsigned int vertexShader, unsigned int fragmentShader) {
    // Attach shaders
    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);

    // Link program
    glLinkProgram(shaderID);

    // Check if linking was succesful
    int linkingSuccess;
    char linkingInfoLog[512];
    glGetProgramiv(shaderID, GL_LINK_STATUS, &linkingSuccess);
    if (!linkingSuccess) {
        glGetProgramInfoLog(shaderID, 512, NULL, linkingInfoLog);
        std::cout << "ERROR:: Shader program linking failed: " << linkingInfoLog << std::endl;
    }
}

// Destroyer
Shader::~Shader() {
    delete platformComponents;
}