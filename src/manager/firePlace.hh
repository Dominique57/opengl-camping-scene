// Created by dominique on 7/15/21.

#pragma once

#include "lightManager.hh"
#include "particleEmitter.hh"

class FirePlace {
public:
    FirePlace(const glm::vec3 &pos, float rescaleFactor, LightManager &lightManager);

    void bind(const program &program);
    void update();
    void draw();

protected:
    glm::vec3 pos;
    ParticleEmitter particleEmitter;
    unsigned lightId;
    LightManager &lightManager;
};