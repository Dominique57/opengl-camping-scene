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


// Constants
const float ambiantFactor = 0.1;

vec3 computeDiffuseAndSpecular(vec3 worldPos, vec3 worldNormal, vec3 objectColor, float ks, float ns) {
    vec3 resColor = vec3(0);

    for (int i = 0; i < lightsLength; ++i) {
        vec3 toLight = normalize(lights[i].pos - worldPos);
        vec3 normal = normalize(worldNormal);
        vec3 diffuseColor = dot(normal, toLight) * lights[i].color * objectColor;

        vec3 viewDir = normalize(cameraPos - worldPos);
        vec3 reflectDir = normalize(reflect(-toLight, normal));
        vec3 specularColor = pow(max(dot(viewDir, reflectDir), 0.0), ns) * ks * lights[i].color;

        resColor += clamp(diffuseColor, 0, 1) + clamp(specularColor, 0, 1);
    }
    return clamp(resColor, ambiantFactor * objectColor, vec3(1)); // minimum is ambiant light
}

vec4 gammaCorrect(vec3 color) {
    float gamma = 2.2;
    vec3 gammaColor = pow(color, vec3(1.0/gamma));
    return vec4(gammaColor, 1.0);
}

void main() {
    // Get pixel position data
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    float ks = texture(gPosition, texCoords).a;
    vec3 fragNormal = texture(gNormal, texCoords).rgb;
    float ns = texture(gNormal, texCoords).a;
    vec3 color = texture(gAlbedo, texCoords).rgb;

    outColor = vec4(computeDiffuseAndSpecular(fragPos, fragNormal, color, ks, ns), 1);
}