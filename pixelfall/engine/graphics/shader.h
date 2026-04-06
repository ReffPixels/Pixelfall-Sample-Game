// Defines a basic shader class that creates a shader from the path to a pair of vertex and fragment shader files.
// The actual implementation is platform specific.

#pragma once
// Standard Library
#include <string>
#include <filesystem>
#include <memory>

class Shader {
public:
    // Create Shader from vertex and fragment paths.
    Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

    // Getters
    const unsigned int& getID() const { return shaderID; };

    // Methods
    void use();
    void setUniformBool(const std::string& name, bool value) const;
    void setUniformInt(const std::string& name, int value) const;
    void setUniformFloat(const std::string& name, float value) const;
    void setUniformVec2(const std::string& name, float x, float y) const;
    void setUniformVec4(const std::string& name, float x, float y, float z, float w) const;
    void setUniformMat4(const std::string& name, float* matrix) const;

    // Destroyers
    ~Shader();

private:
    // Variables
    unsigned int shaderID;
    std::string vertexShaderCode;
    std::string fragmentShaderCode;

    // Platform Implementation (Each platform fills this struct with their necessary components)
    struct PlatformComponents;
    std::unique_ptr<PlatformComponents> platform;

    // Methods
    std::string Shader::getShaderCode(const std::filesystem::path& path);
};