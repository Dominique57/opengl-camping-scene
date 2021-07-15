#pragma once

#include <GL/gl.h>
#include <glm/fwd.hpp>
#include "wrappers/glmWrapper.hh"


class Camera {
public:
    Camera(GLfloat x, GLfloat y, GLfloat z)
        : cameraPos(x, y, z),
          frontVect(0, 0, -1),
          upVect(0, 1, 0)
    {}

    glm::mat4 getTransform() const;

    void moveCamera(const glm::vec3& offset);

    void rotateCamera(const glm::vec2& movement);

    const glm::vec3& viewCameraPos() const { return cameraPos; }

    const glm::vec3& viewFrontVec() const { return frontVect; }

    void alterMovementSpeed(float percentage) { movementSpeed = movementSpeed * percentage; }

protected:
    glm::vec3 computeRightVect() const {
        return glm::normalize(glm::cross(frontVect, {0, 1, 0}));
    }

private:
    GLfloat movementSpeed = 0.1;
    GLfloat mouseSpeed = 0.05;

    glm::vec3 cameraPos;
    glm::vec3 frontVect;
    glm::vec3 upVect;
    GLfloat vertAngle = 0;
    GLfloat horizAngle = -90;
};
