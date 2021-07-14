#include "lightManager.hh"

LightManager::LightManager(unsigned int maxLights)
        : maxLights(maxLights), ssboId(0) {
    lights.reserve(maxLights);

    glGenBuffers(1, &ssboId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, maxLights * sizeof(Light), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboId); // link to binding=0 in shader

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightManager::updateLights() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboId);
    glm::vec4 ssboLength(lights.size());
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 4 * sizeof(GLfloat), &(ssboLength[0]));
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(GLfloat), lights.size() * sizeof(Light), lights.data());

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

unsigned LightManager::addLight(const glm::vec3 &pos, const glm::vec3 &color) {
    if (lights.size() >= maxLights) {
        throw std::logic_error("Adding more lights than maximumn allowed !");
    }

    lights.emplace_back(pos, color);
    return lights.back().id;
}

void LightManager::removeLight(unsigned int id) {
    for (auto it = lights.begin(); it != lights.end();) {
        if (it->id == id) {
            it = lights.erase(it);
            break;
        } else {
            ++it;
        }
    }
}

LightManager::Light &LightManager::getLight(unsigned id) {
    for (auto& light : lights)
        if (light.id == id)
            return light;
    throw std::invalid_argument("LightManager::getLight : Specified light id does not exist !");
}
