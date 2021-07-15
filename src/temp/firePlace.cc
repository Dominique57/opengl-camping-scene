// Created by dominique on 7/15/21.

#include "firePlace.hh"

FirePlace::FirePlace(const glm::vec3 &pos, float rescaleFactor, LightManager &lightManager)
    : pos(pos), particleEmitter(pos, rescaleFactor), lightId(0), lightManager(lightManager) {
    auto lightColor = glm::vec3{
        226.f / 255,
        88.f / 255,
        34.f / 255
    };
    auto lightPos = pos;
    lightPos.y += 3 * rescaleFactor;
    lightId = lightManager.addLight(lightPos, lightColor);
    lightManager.updateLights();
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