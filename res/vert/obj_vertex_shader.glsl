#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 transform_matrix;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = transform_matrix * model * vec4(aPos, 1.0);
}