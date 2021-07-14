#version 460 core

uniform mat4 transform_matrix;
uniform vec3 cameraPos;

in vec3 vPosition;
in vec3 vNormal;
out vec3 fragPos;
out vec3 fragNormal;

void main() {
    vec4 position = vec4(vPosition, 1.0f);
    gl_Position = transform_matrix * position;
    fragNormal = vNormal;
    fragPos = position.xyz;
}
