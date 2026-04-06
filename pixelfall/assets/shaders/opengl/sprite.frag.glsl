#version 330 core
out vec4 FragColor;
  
in vec2 TextureUV;

uniform sampler2D customTexture;
uniform vec4 tintColor;

void main()
{
    FragColor = texture(customTexture, TextureUV) * tintColor;
}