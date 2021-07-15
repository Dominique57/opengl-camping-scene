// Created by dominique on 7/15/21.

#pragma once

#include "wrappers/glmWrapper.hh"
#include "program.hh"


class ParticleEmitter {
public:
    struct ParticleRender {
        glm::vec3 position;
        glm::vec3 color;
        GLfloat size;
        GLfloat lifeLeft = 1.f;
    };
public:
    explicit ParticleEmitter(const glm::vec3 &position, float rescaleFactor);
    void bind(const program &program);
    void update(double timePassed);
    void draw();
    void emit(unsigned count);

protected:
    GLuint vaoId;
    GLuint vboData;
    glm::vec3 emitterPos;
    float rescaleFactor;
    std::vector<ParticleRender> points;
};

