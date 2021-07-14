#version 460 core

struct Light {
    vec3 pos;
    uint id;
    vec3 color;
    float padding2;
};

layout(std430, binding = 0) buffer Lights {
    uint lightsLength;
    Light lights[];
};

uniform mat4 transform_matrix;

in vec3 vPosition;
out vec3 color;

void main() {
    vec4 position = vec4(vPosition, 1.0f);
    gl_Position = transform_matrix * position;
//    color = vec3(1, 0, 0);
    color = vec3(lights[0].pos);
}
