// Created by dominique on 7/15/21.

#pragma once

#include "wrappers/glmWrapper.hh"
#include <temp/program.hh>


class GpuParticleEmitter {
public:
    struct ParticleRender {
        glm::vec3 originalPosition;
        GLfloat originalLifeLeft;
        glm::vec3 position;
        GLfloat size;
        glm::vec3 color;
        GLfloat lifeLeft = 1.f;
    };
public:
    explicit GpuParticleEmitter(const glm::vec3 &position, float rescaleFactor);

    void bind_fragment(const program &program);
    void bind_compute(const program &program);
    void update();
    void draw(const program &program);

private:
    std::vector<ParticleRender> create_particles();

protected:
    GLuint vaoId;
    GLuint vboData;
    unsigned particleCount = 600;

    glm::vec3 emitterPos;
    float rescaleFactor;
};

