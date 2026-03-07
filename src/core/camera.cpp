//
// Created by sameer on 3/7/26.
//

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        position += front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        position -= front * velocity;
    if (direction == Camera_Movement::LEFT)
        position -= right * velocity;
    if (direction == Camera_Movement::RIGHT)
        position += right * velocity;

    position.y = 0.0f; // lock the camera to the ground plane
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