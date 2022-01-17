#version 460 core

#define SAMPLE_SIZE 64

layout (location = 0) uniform sampler2D gPosition;
layout (location = 1) uniform sampler2D gNormal;
layout (location = 2) uniform sampler2D noiseTex;

uniform mat4 projection_matrix;
uniform vec3 kernel[SAMPLE_SIZE];
uniform vec2 noiseScale;

// I/O
in vec2 texCoords;
out float outColor;

// Constants
const float radius = 5;
const float biais = 0.00025;

void main() {
    // Get pixel position data
    vec3 fragPos = texture(gPosition, texCoords).xyz;
    vec3 fragNormal = normalize(texture(gNormal, texCoords).xyz);
    vec3 randomVec = normalize(texture(noiseTex, texCoords * noiseScale).xyz);

    // Gramm-Schmidt process
    vec3 tangent = normalize(randomVec - fragNormal * dot(randomVec, fragNormal));
    vec3 bitangent = cross(fragNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, fragNormal);

    // Compute occlusion
    float occlusion = 0.0f;
    for (uint i = 0; i < SAMPLE_SIZE; ++i) {
        // Get
        vec3 kernelPos = fragPos + (TBN * kernel[i]) * radius; // get kernel position in camera space
        vec4 screenPos = projection_matrix * vec4(kernelPos, 1); // get kernel position in screen space
        screenPos.xyz /= screenPos.w; // Perspective divide for 3d correctness
        screenPos.xyz = screenPos.xyz * 0.5 + 0.5; // Move from [-1;1] to [0;1] range

        // Get kernel depth
        float kernelDepth = texture(gPosition, screenPos.xy).z;
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - kernelDepth));
        occlusion += ((kernelDepth >= kernelPos.z + biais)? 1.0: 0.0) * rangeCheck;
    }
    outColor = 1.0 - (occlusion / SAMPLE_SIZE);
}