#version 330 core

// Sprite Vertex Shader
// Passes UV coordinates to the fragment shader for texture sampling.

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 TextureUV;

uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * vec4(aPos, 1.0);
    TextureUV = aUV;
}