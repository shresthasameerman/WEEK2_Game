//
// Created by sameer on 3/7/26.
//

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>


glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime, std::function<bool(float, float)> isWall)
{
    float velocity = movementSpeed * deltaTime;
    glm::vec3 newPos = position;
    if (direction == Camera_Movement::FORWARD)
        newPos += front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        newPos -= front * velocity;
    if (direction == Camera_Movement::LEFT)
        newPos -= right * velocity;
    if (direction == Camera_Movement::RIGHT)
        newPos += right * velocity;

    float radius = 0.2f; // collision radius

    if (!isWall(newPos.x + radius,position.z) && !isWall(newPos.x - radius, position.z))
        position.x = newPos.x;

    if (!isWall(position.x, newPos.z + radius) && !isWall(position.x, newPos.z - radius))
        position.z = newPos.z;

    position.y = 0.5f; // lock the camera to the ground plane
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
    fov -= yOffset;
    if (fov < 30.0f) fov = 30.0f;
    if (fov > 110.0f) fov = 110.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}