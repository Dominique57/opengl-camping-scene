// Created by dominique on 7/15/21.

#include "firePlace.hh"
#include <random>

FirePlace::FirePlace(const glm::vec3 &pos, float rescaleFactor, LightManager &lightManager)
    : pos(pos), particleEmitter(pos, rescaleFactor), lightId(0), lightManager(lightManager) {
    auto lightColor = glm::vec3{
        226.f / 255,
        88.f / 255,
        34.f / 255
    };
    auto lightPos = pos;
    lightPos.y += rescaleFactor;
    lightId = lightManager.addLight(lightPos, lightColor);
    lightManager.updateLights();
}

void FirePlace::bind(const program &program) {
    particleEmitter.bind(program);
}

void FirePlace::update() {
    // Update light
    static auto intensityOne = glm::vec3{170.f, 66.f, 26.f} / 255.f;
    static auto intensityTwo = glm::vec3{240.f, 110.f, 40.f} / 255.f;
    static unsigned noOfUpdatesNeeded = 0;
    static unsigned noOfUpdates = 0;

    if (noOfUpdates == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> unif(0, 10);

        unsigned randomNoOfUpdates = static_cast<int>(unif(gen)) * 10 + 50;
        noOfUpdatesNeeded = randomNoOfUpdates;
        noOfUpdates = noOfUpdatesNeeded;
        std::swap(intensityOne, intensityTwo);
    }
    float updatedFactor = (float)noOfUpdates / (float)noOfUpdatesNeeded;
    auto lightColor = glm::lerp(intensityOne, intensityTwo, 1 - updatedFactor);
    LightManager::Light& light = lightManager.getLight(lightId);
    light.lightColor = lightColor;
    lightManager.updateLights();
    noOfUpdates -= 1;

//    particleEmitter.update(0.004);
//    particleEmitter.emit(4);
}

void FirePlace::draw(const program &program) {
//    particleEmitter.draw(program);
}