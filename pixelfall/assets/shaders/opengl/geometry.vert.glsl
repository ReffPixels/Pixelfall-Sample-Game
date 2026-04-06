#version 330 core

// Geometry Vertex Shader
// Simple shader that draws geometry with a flat colour.

layout (location = 0) in vec3 aPos;

uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * vec4(aPos, 1.0);
}