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

uniform mat4 view_matrix;
layout(location = 0) uniform sampler2D gPosition;
layout(location = 1) uniform sampler2D gNormal;
layout(location = 2) uniform sampler2D gAlbedo;
layout(location = 3) uniform sampler2D ssaoOcclusion;

// I/O
in vec2 texCoords;
out vec4 outColor;


// Constants
const float ambiantFactor = 0.1;

vec3 computeDiffuseAndSpecular(vec3 viewPos, vec3 viewNormal, vec3 objectColor, float ks, float ns) {
    vec3 resColor = vec3(0);

    for (int i = 0; i < lightsLength; ++i) {
        vec3 lightPos = (view_matrix * vec4(lights[i].pos, 1)).xyz;
        vec3 toLight = normalize(lightPos - viewPos);
        vec3 normal = normalize(viewNormal);
        vec3 diffuseColor = dot(normal, toLight) * lights[i].color * objectColor;

        vec3 viewDir = normalize(-viewPos);
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
    float occlusion = texture(ssaoOcclusion, texCoords).x;
//    outColor = vec4(occlusion, occlusion, occlusion, 1);

    outColor = vec4(computeDiffuseAndSpecular(fragPos, fragNormal, color, ks, ns), 1);
}