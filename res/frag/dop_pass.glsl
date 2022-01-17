#version 460 core

#define SAMPLE_SIZE 64

layout (location = 0) uniform sampler2D gPosition;
layout (location = 1) uniform sampler2D image;

uniform vec3 focusSettings = vec3(-30, 15, 5);
uniform vec2 resolution;
// I/O
in vec2 texCoords;
out vec4 outColor;

const int iter = 5;
const int boxCount = (iter * 2 + 1) * (iter * 2 + 1);

vec3 blur() {
    vec3 sum = vec3(0.0);
    for (int i = -iter; i <= iter; i++) {
        for (int j = -iter; j <= iter; j++) {
            sum += texture(image, texCoords + (vec2(i, j) / resolution)).rgb;
        }
    }
    sum = clamp((sum / boxCount), vec3(0), vec3(1));
    return sum;
}

void main() {
    float depth = texture(gPosition, texCoords).z;
    vec3 color = texture(image, texCoords).rgb;
    vec3 colorBlur = blur();
    vec3 resColor;

    float lengthFromFocus = abs(focusSettings.x - depth);
    if (lengthFromFocus < focusSettings.y) {
        resColor = color;
    } else if (lengthFromFocus < focusSettings.y + focusSettings.z) {
        resColor = mix(color, colorBlur, (lengthFromFocus - focusSettings.y) / focusSettings.z);
    } else {
        resColor = colorBlur;
    }

    outColor = vec4(resColor, 1);
}