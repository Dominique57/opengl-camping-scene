#version 460 core

uniform mat4 transform_matrix;

in vec3 vPosition;
out vec3 color;

void main() {
    vec4 position = vec4(vPosition, 1.0f);
    gl_Position = transform_matrix * position;
    color = vec3(1, 0, 0);
}
