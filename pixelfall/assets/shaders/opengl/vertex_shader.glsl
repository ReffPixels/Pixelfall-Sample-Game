#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * vec4(aPos, 1.0);
}