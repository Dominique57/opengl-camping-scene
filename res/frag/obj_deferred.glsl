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

uniform vec3 texture_coef;
uniform vec3 cameraPos;
uniform mat4 view_matrix;
uniform sampler2D texture_diffuse1;

// I/O
in vec2 texCoords;
in vec3 fragPos;
in vec3 fragNormal;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

// Constants
const float ambiantFactor = 0.1;

vec3 computeDiffuseAndSpecular(vec3 objectColor) {
    float diffusionFactor = texture_coef.x;
    float specularFactor = texture_coef.y;
    float specularShininess = texture_coef.z;

    vec3 resColor = vec3(0);

    for (int i = 0; i < lightsLength; ++i) {
        vec3 lightPos = vec3(view_matrix * vec4(lights[i].pos, 1.0));
        vec3 toLight = normalize(lightPos - fragPos);
        vec3 normal = normalize(fragNormal);
        vec3 diffuseColor = dot(normal, toLight) * lights[i].color * diffusionFactor * objectColor;

        vec3 viewDir = normalize(cameraPos - fragPos);
        vec3 reflectDir = normalize(reflect(-toLight, normal));
        vec3 specularColor = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess)
        * specularFactor * lights[i].color; // * specularFactor

        resColor += clamp(diffuseColor, 0, 1) + clamp(specularColor, 0, 1);
    }
    vec3 minColor = ambiantFactor * objectColor; // minimum is ambiant light
    return clamp(resColor, minColor, vec3(1));
}

vec4 gammaCorrect(vec3 color) {
    float gamma = 2.2;
    vec3 gammaColor = pow(color, vec3(1.0/gamma));
    return vec4(gammaColor, 1.0);
}

void main() {
    vec4 color = texture(texture_diffuse1, texCoords);
//    out_color = vec4(computeDiffuseAndSpecular(color.xyz), color.w);

    gPosition = fragPos;
    gNormal = fragNormal;
    gAlbedo = computeDiffuseAndSpecular(color.xyz);
}