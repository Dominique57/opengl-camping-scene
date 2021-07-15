// Created by dominique on 7/15/21.

#include "firePlace.hh"

FirePlace::FirePlace(const glm::vec3 &pos, float rescaleFactor, LightManager &lightManager)
    : pos(pos), particleEmitter(pos, rescaleFactor), lightManager(lightManager) {
}

void FirePlace::bind(const program &program) {
    particleEmitter.bind(program);
}

void FirePlace::update() {
    particleEmitter.update(0.004);
    particleEmitter.emit(4);
}

void FirePlace::draw() {
    particleEmitter.draw();
}