// Implementation of shader.h for OpenGL.

#include "pixelfall/engine/graphics/shader.h"
// Standard Library
#include <fstream>
#include <sstream>
#include <iostream>
// OpenGL
#include <glad/glad.h>

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
    platform = std::make_unique<PlatformComponents>();

    // Get shader code (OpenGL needs this in const char* format)
    vertexShaderCode = getShaderCode(vertexShaderPath);
    fragmentShaderCode = getShaderCode(fragmentShaderPath);
    platform->vertexShaderCodeOpenGL = vertexShaderCode.c_str();
    platform->fragmentShaderCodeOpenGL = fragmentShaderCode.c_str();

    // Create Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    configureShader(vertexShader, platform->vertexShaderCodeOpenGL);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    configureShader(fragmentShader, platform->fragmentShaderCodeOpenGL);

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

// Set value of a bool uniform in the shader
void Shader::setUniformBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}
// Set value of an int uniform in the shader
void Shader::setUniformInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}
// Set value of a float uniform in the shader
void Shader::setUniformFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}
// Set value of a Vector 2 uniform in the shader
void Shader::setUniformVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(shaderID, name.c_str()), x, y);
}
// Set value of a Vector 4 uniform in the shader
void Shader::setUniformVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(shaderID, name.c_str()), x, y, z, w);
}
// Set value of a Matrix 4 float uniform in the shader
void Shader::setUniformMat4(const std::string& name, float* matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, matrix);
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
Shader::~Shader() = default;