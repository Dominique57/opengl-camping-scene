#pragma once

#include <vector>
#include "wrappers/glWrapper.hh"
#include "wrappers/glmWrapper.hh"

class LightManager {
public:
    struct Light {
        Light(const glm::vec3 &pos, const glm::vec3 &color)
            : lightPos(pos), lightColor(color) {
            static unsigned ctorCounter = 0;
            id = ctorCounter++;
        }

        glm::vec3 lightPos; // GLfloat padding1;
        GLuint id;
        glm::vec3 lightColor;
        GLfloat padding2;
    };

    LightManager(unsigned maxLights=20);

    void updateLights();

    unsigned addLight(const glm::vec3 &pos, const glm::vec3 &color);

    void removeLight(unsigned id);

    Light& getLight(unsigned id);

protected:
    unsigned maxLights;
    GLuint ssboId;
    std::vector<Light> lights;
};