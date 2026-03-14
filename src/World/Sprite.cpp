#include "Sprite.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Level.h"

// static members
unsigned int Sprite::s_texture   = 0;
unsigned int Sprite::s_VAO       = 0;
unsigned int Sprite::s_VBO       = 0;
bool         Sprite::s_meshReady = false;

Sprite::Sprite(glm::vec3 pos) : position(pos) {}

Sprite::~Sprite() {}

void Sprite::buildMesh()
{
    float verts[] = {
        -0.5f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f, 1.0f, 0.0f,  1.0f, 1.0f,

        -0.5f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, 1.0f, 0.0f,  0.0f, 1.0f,
    };

    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);
    glBindVertexArray(s_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    s_meshReady = true;
}

bool Sprite::loadTexture(const std::string& path)
{
    if (!s_meshReady) buildMesh();

    glGenTextures(1, &s_texture);
    glBindTexture(GL_TEXTURE_2D, s_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int w, h, ch;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 4);
    if (!data) {
        std::cerr << "Failed to load sprite texture: " << path << "\n";
        return false;
    }
    std::cout << "Sprite texture loaded: " << w << "x" << h << "\n";
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return true;
}

void Sprite::draw(Shader& shader, glm::vec3 cameraPos, float deltaTime)
{
    if (!alive) return;

    // chase player
    glm::vec3 dir = cameraPos - position;
    dir.y = 0.0f;
    float dist = glm::length(dir);
    if (dist > 0.8f && dist < 8.0f) {
        glm::vec3 move = glm::normalize(dir) * 1.5f * deltaTime;
        glm::vec3 newPos = position + move;

        // wall check
        int col = (int)newPos.x;
        int row = (int)newPos.z;
        if (row >= 0 && row < MAP_HEIGHT && col >= 0 && col < MAP_WIDTH && MAP[row][col] == 0)
            position = newPos;
    }

    // billboard - face the camera
    float angle = glm::degrees(atan2(dir.x, dir.z));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

    if (hitFlashTimer > 0.0f) {
        hitFlashTimer -= deltaTime;
        shader.setVec3("uTint", glm::vec3(5.0f, 0.0f, 0.0f)); // full red
    } else {
        shader.setVec3("uTint", glm::vec3(1.0f, 1.0f, 1.0f));
    }
    shader.setMat4("uModel", model);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_texture);
    shader.setInt("uTexture", 0);
    shader.setBool("uUseTexture", true);

    glBindVertexArray(s_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}