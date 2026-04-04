// Defines a basic mesh that is drawn with shader triangles. Used by painter to draw shapes on the screen. 
// The actual implementation is platform specific.

#pragma once
#include <vector>
#include <memory>

class Mesh {
public:
    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);

    // Setters
    void setVertices(std::vector<float> vertices) { this->vertices = vertices; };
    void setIndices(std::vector<unsigned int> indices) { this->indices = indices; };

    // Getters
    const std::vector<float>& getVertices() const { return vertices; };
    const std::vector<unsigned int>& getIndices() const { return indices; };

    // Methods
    void update();
    void draw();

    // Destroyers
    ~Mesh();
private:
    // Variables
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Platform Implementation (Each platform fills this struct with their necessary components)
    struct PlatformComponents;
    std::unique_ptr<PlatformComponents> platform;
};