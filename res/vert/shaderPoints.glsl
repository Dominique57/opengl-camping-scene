#version 460 core

uniform mat4 transform_matrix;

in vec3 vPosition;
in vec3 vColor;
in float vSize;
out vec3 color;

void main() {
    vec4 position = vec4(vPosition, 1.0f);
    gl_Position = transform_matrix * position;
    color = vec3(vColor);

    gl_PointSize = (1 / gl_Position.z) * 300 * vSize;
}
