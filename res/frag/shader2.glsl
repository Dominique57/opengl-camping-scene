#version 460 core

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

in vec3 fragNormal;
in vec3 fragPos;

out vec4 out_color;

float diffusionFactor = 0.7;
float specularFactor = 0.8;
float specularShininess = 32;
float ambiantFactor = 0.1;
vec3 computeDiffuseAndSpecular(vec3 objectColor) {
    vec3 resColor = vec3(0);

    for (int i = 0; i < 2; ++i) {
        vec3 toLight = normalize(lights[i].pos - fragPos);
        vec3 normal = normalize(fragNormal);
        vec3 diffuseColor = dot(normal, toLight) * lights[i].color * diffusionFactor * objectColor;

        vec3 viewDir = normalize(cameraPos - fragPos);
        vec3 reflectDir = normalize(reflect(-toLight, normal));
        vec3 specularColor = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess)
            * specularFactor * lights[i].color; // * specularFactor

        resColor += clamp(diffuseColor, 0, 1) + clamp(specularColor, 0, 1);
    }
    vec3 minColor = ambiantFactor * vec3(1, 1, 1); // minimum is ambiant light
    return clamp(resColor, minColor, vec3(1));
}

vec4 gammaCorrect(vec3 color) {
    float gamma = 2.2;
    vec3 gammaColor = pow(color, vec3(1.0/gamma));
    return vec4(gammaColor, 1.0);
}

void main() {
    out_color = gammaCorrect(computeDiffuseAndSpecular(vec3(1, 1, 1)));
//    out_color = vec4(computeDiffuseAndSpecular(), 1);
//    out_color = vec4((gl_FragCoord.xyz + 20) / 40, 1);
}
