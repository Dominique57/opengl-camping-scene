// Created by dominique on 7/15/21.

#pragma once

#include "temp/program.hh"
#include "wrappers/glmWrapper.hh"
#include "particleEmitter.hh"
#include "lightManager.hh"


class FireworkEmitter {
public:
    struct ParticleRender {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        GLfloat size;
        GLfloat lifeLeft = 1.f;
        GLboolean generateParticles = false;
    };

    struct FireWorkLight {
        unsigned lightId;
        float life;
    };

public:
    FireworkEmitter(const glm::vec3 &position, const glm::vec3 &color,
                    float rescaleFactor, LightManager &lightmanager);
    void bind(const program &program);
    void update(double timePassed);
    void draw();
    void emit(unsigned count);
    unsigned particleCount() const { return points.size(); }

    void setPosition(const glm::vec3 &pos) { emitterPos = pos; }
    const glm::vec3& getPosition() const { return emitterPos; }

    void setParticleColor(const glm::vec3 &color) { particleColor = color; }
    const glm::vec3& getParticleColor() const { return particleColor; }

protected:
    GLuint vaoId;
    GLuint vboData;
    glm::vec3 emitterPos;
    glm::vec3 particleColor;
    float rescaleFactor;
    LightManager &lightmanager;
    std::vector<ParticleRender> points;
    std::vector<FireWorkLight> lights;
};

