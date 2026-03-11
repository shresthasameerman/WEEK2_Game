#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool load(const std::string& objPath, const std::string& texturePath = "");
    void draw(Shader& shader);

private:
    unsigned int m_VAO      = 0;
    unsigned int m_VBO      = 0;
    unsigned int m_texture  = 0;
    int          m_vertexCount = 0;

    bool loadTexture(const std::string& path);
};