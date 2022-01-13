#version 460 core

in vec3 color;
in vec3 normal;
in vec3 toLight;
uniform vec3 lightColor;

out vec4 out_color;

void main() {
    out_color = vec4(
        clamp(color * dot(normal, toLight) * lightColor, 0.0, 1.0),
        1.0
    );
}
