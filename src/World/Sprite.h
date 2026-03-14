#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "core/Shader.h"

class Sprite {
public:
    glm::vec3 position;
    float     health   = 100.0f;
    bool      alive    = true;
    float hitFlashTimer = 0.0f; // counts down after being hit

    Sprite(glm::vec3 pos);
    ~Sprite();

    static bool loadTexture(const std::string& path);
    void draw(Shader& shader, glm::vec3 cameraPos, float deltaTime);

private:
    static unsigned int s_texture;
    static unsigned int s_VAO;
    static unsigned int s_VBO;
    static bool         s_meshReady;
    static void         buildMesh();
};