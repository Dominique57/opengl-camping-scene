#version 460 core

out vec3 TexCoords;

uniform mat4 transform_matrix;
in vec3 vPosition;

void main()
{
    TexCoords = vPosition;
    vec4 position = vec4(vPosition, 1.0f);
    gl_Position = transform_matrix * position;
}