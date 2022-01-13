// Created by dominique on 7/15/21.

#pragma once

#include "wrappers/glmWrapper.hh"
#include <temp/program.hh>


class GpuParticleEmitter {
public:
    struct ParticleRender {
        glm::vec3 position;
        glm::vec3 color;
        GLfloat size;
        GLfloat lifeLeft = 1.f;
    };
public:
    explicit GpuParticleEmitter(const glm::vec3 &position, float rescaleFactor);

    void bind(const program &program);
    void update(const program& computeShader);
    void draw();

//private:
    void init_particles();

protected:
    GLuint vaoId;
    GLuint vboData;
    unsigned particleCount = 1;

    glm::vec3 emitterPos;
    float rescaleFactor;
};

