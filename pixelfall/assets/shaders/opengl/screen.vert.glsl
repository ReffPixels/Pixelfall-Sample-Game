#version 330 core

// Screen Vertex Shader
// Passes NDC positions and UV coordinates to the fragment shader.
// Used by all screen-space passes (MSAA and FXAA).

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
