#version 460 core

in vec3 vPosition;
in vec3 vNormal;
in vec3 vColor;

void main() {
    vec4 position = vec4(vPosition, 1.0f);
    gl_Position = transform_matrix * position;
}
