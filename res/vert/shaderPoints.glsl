#version 460 core

uniform mat4 transform_matrix;
uniform vec4 transform_pos_size = vec4(0, 0, 0, 1);

in vec3 vPosition;
in vec3 vColor;
in float vSize;
out vec3 color;

void main() {
    vec3 worldPos = vPosition * transform_pos_size.w + transform_pos_size.xyz;
    float size = vSize * transform_pos_size.w;

    gl_Position = transform_matrix * vec4(worldPos, 1);
    color = vec3(vColor);

    gl_PointSize = (1 / gl_Position.z) * 300 * size;
}
