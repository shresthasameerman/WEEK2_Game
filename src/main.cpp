#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "core/Shader.h"
#include "World/Level.h"
#include "core/mesh.h"
#include "World/Sprite.h"
#include <vector>

const char* VERT_PATH     = "assets/shaders/world.vert";
const char* FRAG_PATH     = "assets/shaders/world.frag";
const char* GUN_VERT_PATH = "assets/shaders/gun.vert";
const char* GUN_FRAG_PATH = "assets/shaders/gun.frag";

const unsigned int SCR_WIDTH  = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(1.5f, 0.5f, 1.5f));
float lastX = SCR_WIDTH  / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll(yOffset);
}

bool mouseFired = false;
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouseFired = true;
}

int main()
{
    std::cout << "DoomWeek engine starting...\n";

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "DOOMWEEK", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    Shader worldShader(VERT_PATH, FRAG_PATH);
    Shader gunShader(GUN_VERT_PATH, GUN_FRAG_PATH);

    Level level;
    level.build();

    Mesh gunMesh;
    if (!gunMesh.load(
        "/home/sameer/Documents/C++GameDEV/DoomWeek/assets/models/Pistol/PistolFinal.obj",
        "/home/sameer/Documents/C++GameDEV/DoomWeek/assets/models/Pistol/ZenUV_Generic_Material.001_BaseColor.1001.png"))
        std::cerr << "Failed to load gun\n";

    Sprite::loadTexture("assets/textures/enemy.png");
    std::vector<Sprite> enemies;
    enemies.emplace_back(glm::vec3(5.5f, 0.0f, 5.5f));
    enemies.emplace_back(glm::vec3(3.5f, 0.0f, 7.5f));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(Camera_Movement::FORWARD, deltaTime, [](float x, float z) {
                int col = (int)x; int row = (int)z;
                if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH) return true;
                return MAP[row][col] == 1;
            });
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime, [](float x, float z) {
                int col = (int)x; int row = (int)z;
                if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH) return true;
                return MAP[row][col] == 1;
            });
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(Camera_Movement::LEFT, deltaTime, [](float x, float z) {
                int col = (int)x; int row = (int)z;
                if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH) return true;
                return MAP[row][col] == 1;
            });
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(Camera_Movement::RIGHT, deltaTime, [](float x, float z) {
                int col = (int)x; int row = (int)z;
                if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH) return true;
                return MAP[row][col] == 1;
            });

        // shooting
        if (mouseFired) {
            mouseFired = false;
            for (auto& enemy : enemies) {
                if (!enemy.alive) continue;
                glm::vec3 toEnemy = enemy.position - camera.position;
                toEnemy.y = 0.0f;
                float dist = glm::length(toEnemy);
                if (dist > 10.0f) continue;

                glm::vec3 camFront = camera.front;
                camFront.y = 0.0f;
                camFront = glm::normalize(camFront);
                glm::vec3 enemyDir = glm::normalize(toEnemy);

                float dot = glm::dot(camFront, enemyDir);
                if (dot > 0.85f) {
                    enemy.health -= 34.0f;
                    enemy.hitFlashTimer = 0.4f;
                    std::cout << "HIT! Enemy health: " << enemy.health << "\n";
                    if (enemy.health <= 0.0f) {
                        enemy.alive = false;
                        std::cout << "ENEMY DEAD!\n";
                    }
                    break;
                }
            }
        }

        glClearColor(0.1f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render level
        worldShader.use();
        worldShader.setFloat("uTime", (float)glfwGetTime());
        worldShader.setMat4("uModel", glm::mat4(1.0f));
        worldShader.setMat4("uView", camera.getViewMatrix());
        worldShader.setMat4("uProjection", glm::perspective(
            glm::radians(camera.fov),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
        level.draw(worldShader);

        // render enemies
        gunShader.use();
        gunShader.setMat4("uView", camera.getViewMatrix());
        gunShader.setMat4("uProjection", glm::perspective(
            glm::radians(camera.fov),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
        for (auto& enemy : enemies)
            enemy.draw(gunShader, camera.position, deltaTime);

        // render gun
        glClear(GL_DEPTH_BUFFER_BIT);
        gunShader.use();

        glm::mat4 gunModel = glm::mat4(1.0f);
        gunModel = glm::translate(gunModel, glm::vec3(0.38f, -0.22f, -0.5f));
        gunModel = glm::rotate(gunModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        gunModel = glm::rotate(gunModel, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        gunModel = glm::scale(gunModel, glm::vec3(0.025f, 0.025f, 0.025f));

        gunShader.setMat4("uModel", gunModel);
        gunShader.setMat4("uView", glm::mat4(1.0f));
        gunShader.setMat4("uProjection", glm::perspective(
            glm::radians(60.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 10.0f));
        gunMesh.draw(gunShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}