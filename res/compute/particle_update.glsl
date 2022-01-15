#version 460 core

layout (local_size_x=256) in;

struct ParticleRender {
    vec3 position;
    float size;
    vec3 color;
    float lifeLeft;
};

layout(std430, binding = 79) buffer Particles {
    ParticleRender particles[];
};

struct LerpColor {
    vec3 color;
    float limit;
};

#define LERP_COLORS_SIZE 10
const LerpColor[LERP_COLORS_SIZE] lerp_colors = {
    { { 0.35, 0.3, 0.3 }, 0.0},
    { { 0.35, 0.3, 0.3 }, 0.1},
    { { 0.4, 0.2, 0.15 }, 0.2},
    { { 0.2, 0.1, 0.0 }, 0.3},
    { { 0.2, 0.07, 0.0 }, 0.8},
    { { 0.1, 0.02, 0.0 }, 1.4},
    { { 0, 0, 0 }, 1.8},
    { { 0, 0, 0 }, 2.2},
    { { 0.15, 0.15, 0.15}, 3.2},
    { { 0.05, 0.05, 0.05}, 4.0},
};

vec3 updateColor(vec3 pPos)
{
    // Compute distanceToCenterFire and associated lerpColor index
    vec3 refPos = vec3(pPos.x * 2, pPos.y - 0.4, pPos.z * 2);
    float distFireCenter = length(refPos - vec3(0, 0, 0));
    uint lerpColorIndex = 1;
    for (; lerpColorIndex < LERP_COLORS_SIZE; ++lerpColorIndex)
        if (distFireCenter < lerp_colors[lerpColorIndex].limit)
            break;

    // Calculate lerp between 2 colors
    LerpColor minColor = lerp_colors[lerpColorIndex - 1];
    LerpColor maxColor = lerp_colors[lerpColorIndex];
    float factor = (distFireCenter - minColor.limit) / (maxColor.limit - minColor.limit);
    vec3 newColor = mix(minColor.color, maxColor.color, factor);

    return newColor;
}

vec3 updatePosition(vec3 pPos)
{
    vec3 newPos = pPos;

    // Vertical update
    float upSpeed = 0.01; // [0.005-0.015]
    newPos.y = (pPos.y > 4)? 0: pPos.y + upSpeed;

    // Horizontal update
    vec3 posEmitterPlane = vec3(0, 0, 0);
    vec3 posInEmitterPlane = vec3(pPos.x, 0, pPos.z);
    vec3 particleDir = normalize(posEmitterPlane - posInEmitterPlane);
    float lengthToEmitter = length(particleDir);
    lengthToEmitter = (pPos.y > 2)? lengthToEmitter: -lengthToEmitter;
    newPos.x += particleDir.x * lengthToEmitter * 0.0025f;
    newPos.z += particleDir.z * lengthToEmitter * 0.0025f;

    return newPos;
}

void main() {
    // Input data
    uint i = gl_GlobalInvocationID.x;
    vec3 pPos = particles[i].position;

    // Compute new data
    vec3 newPos = updatePosition(pPos);
    vec3 newColor = updateColor(newPos);

    // Write new data
    particles[i].position = newPos;
    particles[i].color = newColor;
}