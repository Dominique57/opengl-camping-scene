#version 460 core

// External data
struct Light {
    vec3 pos;
    uint id;
    vec3 color;
    float padding2;
};

layout(std430, binding = 0) buffer Lights {
    vec3 padding;
    uint lightsLength;
    Light lights[];
};

uniform vec3 cameraPos;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

// I/O
in vec2 texCoords;
out vec4 outColor;

vec4 gammaCorrect(vec3 color) {
    float gamma = 2.2;
    vec3 gammaColor = pow(color, vec3(1.0/gamma));
    return vec4(gammaColor, 1.0);
}

void main() {
    // Get pixel position data
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 fragNormal = texture(gNormal, texCoords).rgb;
    vec3 color = texture(gAlbedo, texCoords).rgb;

    outColor = vec4(color, 1);
}