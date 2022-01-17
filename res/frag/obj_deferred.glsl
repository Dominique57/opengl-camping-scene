#version 460 core

uniform vec3 texture_coef;
uniform sampler2D texture_diffuse1;

// I/O
in vec2 texCoords;
in vec3 fragPos;
in vec3 fragNormal;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gAlbedo;

void main() {
    vec4 color = texture(texture_diffuse1, texCoords);

    float diffusionFactor = texture_coef.x;
    float specularFactor = texture_coef.y;
    float specularShininess = texture_coef.z;

    gPosition = vec4(fragPos, specularFactor);
    gNormal = vec4(normalize(fragNormal), specularShininess);
    gAlbedo = color.xyz * diffusionFactor;
}