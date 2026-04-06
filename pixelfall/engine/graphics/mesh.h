// Defines a basic mesh that is drawn with shader triangles. Used by painter to draw shapes on the screen. 
// The actual implementation is platform specific.

#pragma once
// Standard Library
#include <vector>
#include <memory>

// Information passed to the GPU to interpret the vertex data. 
// This is necessary to allow for variable amounts of attributes (Not just geometry)
// For example, a Sprite Mesh also needs to pass texture UV information
struct VertexAttribute {
    int components;
};

class Mesh {
public:
    Mesh(
        std::vector<float> vertices,
        std::vector<unsigned int> indices,
        // Default 3 component attribute used for geometry (x, y, z)
        std::vector<VertexAttribute> attributes = {{3}}
    );

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