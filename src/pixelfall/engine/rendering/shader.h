// Defines a basic shader class that creates a shader from the path to a pair of vertex and fragment shader files.
// The actual implementation is platform specific.

#pragma once
#include <string>
#include <filesystem>

class Shader {
public:
    // Create Shader from vertex and fragment paths.
    Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);

    // Getters
    const unsigned int& getID() const { return shaderID; };

    // Methods
    void use();
    void addUniformBool(const std::string& name, bool value) const;
    void addUniformInt(const std::string& name, int value) const;
    void addUniformFloat(const std::string& name, float value) const;

    // Destroyers
    ~Shader();

private:
    // Variables
    unsigned int shaderID;
    std::string vertexShaderCode;
    std::string fragmentShaderCode;

    // Platform Implementation (Each platform fills this struct with their necessary components)
    struct PlatformComponents;
    PlatformComponents* platformComponents = nullptr;

    // Methods
    std::string Shader::getShaderCode(const std::filesystem::path& path);
};