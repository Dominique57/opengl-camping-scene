#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = vec4(inPos, 1);
    texCoords = inTexCoords;
}