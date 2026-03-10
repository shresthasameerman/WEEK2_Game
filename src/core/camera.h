#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH = 0.0f;
const float DEFAULT_SPEED = 5.0f;
const float DEFAULT_SENSITIVITY = 0.1f;
const float DEFAULT_FOV = 90.0f;

class Camera {
    //camera attributes
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    //euler angles
    float yaw;
    float pitch;

    //camera options
    float movementSpeed;
    float mouseSensitivity;
    float fov;

    //constructor with vectors
    Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 startUp = glm::vec3(0.0f, 1.0f, 0.0f),
        float startYaw = DEFAULT_YAW,
        float startPitch = DEFAULT_PITCH) : position(startPosition),
        worldUp(startUp), yaw(startYaw), pitch(startPitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(DEFAULT_SPEED),
        mouseSensitivity(DEFAULT_SENSITIVITY),
        fov(DEFAULT_FOV) {
        updateCameraVectors();
    }

    //returns the view matrix for the renderer
    glm::mat4 getViewMatrix();

    //processes input from keyboard
    void processKeyboard(Camera_Movement direction, float deltaTime, std::function<bool(float, float)> isWall);

    //processes mouse input
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    //processes mouse scroll input
    void processMouseScroll(float yoffset);

private:
    //recalculates front, right and up vectors from yaw and pitch
    void updateCameraVectors();


};