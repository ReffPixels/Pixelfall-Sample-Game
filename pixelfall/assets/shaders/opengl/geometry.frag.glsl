#version 330 core

// Geometry Fragment Shader
// Simple shader that draws geometry with a flat colour.

out vec4 FragColor;

uniform vec4 customColor;

void main()
{
    FragColor = vec4(customColor);
}