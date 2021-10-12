#include "camera.hh"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

void Camera::moveCamera(const glm::vec3& offset) {
    glm::vec3 movement = offset.x * frontVect
        + offset.y * upVect
        + offset.z * glm::cross(frontVect, upVect);
    cameraPos += movement * movementSpeed;
}

void Camera::rotateCamera(const glm::vec2& rawMovement) {
    horizAngle += rawMovement.x * mouseSpeed;
    vertAngle += rawMovement.y * mouseSpeed;

    if (vertAngle > 80)
        vertAngle = 80;
    if (vertAngle < -80)
        vertAngle = -80;
    if (horizAngle > 360)
        horizAngle -= 360;
    if (horizAngle < -360)
        horizAngle += 360;

    frontVect.x = cos(glm::radians(horizAngle)) * cos(glm::radians(vertAngle));
    frontVect.y = sin(glm::radians(vertAngle));
    frontVect.z = sin(glm::radians(horizAngle)) * cos(glm::radians(vertAngle));

    frontVect = glm::normalize(frontVect);
    upVect = glm::normalize(glm::cross(computeRightVect(), frontVect));
}

glm::mat4 Camera::getTransform() const {
    auto viewMat = getView();

    // static const since its value ought not to be changed
    static const auto perspective = glm::perspective(
        glm::radians(65.0f),
        1.1f,
        0.1f,
        600.f
    );

    return perspective * viewMat;
}

glm::mat4 Camera::getView() const {
    auto viewMat = glm::lookAt(
            cameraPos,
            cameraPos + frontVect,
            upVect
    );
    return viewMat;
}
