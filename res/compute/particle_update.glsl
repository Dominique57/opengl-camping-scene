#version 460 core

layout (local_size_x=256) in;

struct ParticleRender {
    vec3 position;
    vec3 color;
    float size;
    float lifeLeft;
};

layout(std430, binding = 79) buffer Particles {
    ParticleRender particles[];
};

void main() {
    uint i = gl_GlobalInvocationID.x;
    float height = particles[i].position.y;
    particles[i].position.y = (height > 2)? 0: height + 0.1;
}