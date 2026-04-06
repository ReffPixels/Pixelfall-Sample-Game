#version 330 core

// MSAA Fragment Shader
// Simple fragment shader that takes the framebuffer texture and outputs it unchanged.
// Used when anti aliasing is provided by hardware MSAA.

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, TexCoord);
}
