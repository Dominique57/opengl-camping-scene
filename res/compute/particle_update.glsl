#version 460 core

layout (local_size_x=256) in;

struct ParticleRender {
    vec3 originalPosition;
    float originalLifeLeft;
    vec3 position;
    float size;
    vec3 color;
    float lifeLeft;
};

layout(std430, binding = 1) buffer Particles {
    ParticleRender particles[];
};

struct LerpColor {
    vec3 color;
    float limit;
};

#define LERP_COLORS_SIZE 10
const LerpColor[LERP_COLORS_SIZE] lerp_colors = {
    { { 0.35, 0.3, 0.3 },   0.0 },
    { { 0.35, 0.3, 0.3 },   0.1 },
    { { 0.4, 0.2, 0.15 },   0.2 },
    { { 0.2, 0.1, 0.0 },    0.3 },
    { { 0.2, 0.07, 0.0 },   0.8 },
    { { 0.1, 0.02, 0.0 },   1.4 },
    { { 0, 0, 0 },          1.8 },
    { { 0, 0, 0 },          2.2 },
    { { 0.15, 0.15, 0.15 }, 3.2 },
    { { 0.05, 0.05, 0.05 }, 4.0 },
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
    newPos.y = pPos.y + upSpeed;

    // Horizontal update
    vec3 posEmitterPlane = vec3(0, 0, 0);
    vec3 posInEmitterPlane = vec3(pPos.x, 0, pPos.z);
    vec3 particleDir = normalize(posEmitterPlane - posInEmitterPlane);
    float lengthToAxis = length(posEmitterPlane - posInEmitterPlane);
    lengthToAxis = (pPos.y < 1)? -lengthToAxis: lengthToAxis;
    lengthToAxis = (pPos.y >= 1 && pPos.y < 2)? lengthToAxis * pPos.y: lengthToAxis;
    newPos.x += particleDir.x * lengthToAxis * 0.0025f;
    newPos.z += particleDir.z * lengthToAxis * 0.0025f;

    return newPos;
}

float updateSize(float lifeLeft)
{
    float lifeSpent = 1 - lifeLeft;
    if (lifeSpent < 0.5)
        return mix(1.6f, 1.2f, lifeSpent * 2.f);
    if (lifeSpent < 0.8)
        return mix(1.2f, 1.0f, (lifeSpent - 0.5) * 3.33f);
    return mix(1.0f, 0.8f, (lifeSpent - 0.8f) * 5.f);
}

float updateLifeLeft(float lifeLeft)
{
    const float frameTime = 0.004;
    return lifeLeft - frameTime;
}

void main() {
    // Input data
    uint i = gl_GlobalInvocationID.x;
    vec3 pPos = particles[i].position;
    float lifeLeft = particles[i].lifeLeft;

    // Check if particle is still alive
    if (lifeLeft <= 0.f) {
        pPos = particles[i].originalPosition;
        lifeLeft = particles[i].originalLifeLeft;
    }

    // Compute new data
    float newLifeLeft = updateLifeLeft(lifeLeft);
    vec3 newPos = updatePosition(pPos);
    vec3 newColor = updateColor(newPos);
    float newSize = updateSize(lifeLeft);

    // Write new data
    particles[i].position = newPos;
    particles[i].size = newSize;
    particles[i].color = newColor;
    particles[i].lifeLeft = newLifeLeft;
}