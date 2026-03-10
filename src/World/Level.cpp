#include "Level.h"
#include <glad/glad.h>

Level::Level() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
}

Level::~Level() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Level::addQuad(std::vector<float>& verts, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 color) {
    // front side
    verts.insert(verts.end(), {
        p1.x, p1.y, p1.z, color.r, color.g, color.b,
        p2.x, p2.y, p2.z, color.r, color.g, color.b,
        p3.x, p3.y, p3.z, color.r, color.g, color.b,
        p1.x, p1.y, p1.z, color.r, color.g, color.b,
        p3.x, p3.y, p3.z, color.r, color.g, color.b,
        p4.x, p4.y, p4.z, color.r, color.g, color.b,
    });
    // back side - same vertices, reversed order
    verts.insert(verts.end(), {
        p4.x, p4.y, p4.z, color.r, color.g, color.b,
        p3.x, p3.y, p3.z, color.r, color.g, color.b,
        p1.x, p1.y, p1.z, color.r, color.g, color.b,
        p3.x, p3.y, p3.z, color.r, color.g, color.b,
        p2.x, p2.y, p2.z, color.r, color.g, color.b,
        p1.x, p1.y, p1.z, color.r, color.g, color.b,
    });
}

void Level::build() {
    std::vector<float> verts;

    glm::vec3 wallNS    = glm::vec3(0.6f, 0.35f, 0.25f); // north/south - darker
    glm::vec3 wallEW    = glm::vec3(0.9f, 0.6f, 0.45f);  // east/west - brighter
    glm::vec3 floorColor   = glm::vec3(0.3f, 0.3f, 0.3f);
    glm::vec3 ceilingColor = glm::vec3(0.12f, 0.12f, 0.12f);


    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            float x = (float)col;
            float z = (float)row;

            // floor and ceiling for empty tiles
            if (MAP[row][col] == 0) {
                addQuad(verts,
                    glm::vec3(x,        0.0f, z),
                    glm::vec3(x + 1.0f, 0.0f, z),
                    glm::vec3(x + 1.0f, 0.0f, z + 1.0f),
                    glm::vec3(x,        0.0f, z + 1.0f),
                    floorColor);

                addQuad(verts,
                     glm::vec3(x,        1.0f, z + 1.0f),
                     glm::vec3(x + 1.0f, 1.0f, z + 1.0f),
                     glm::vec3(x + 1.0f, 1.0f, z),
                     glm::vec3(x,        1.0f, z),
                     ceilingColor);
            }

            // walls on wall tiles that touch empty tiles
            if (MAP[row][col] == 1) {
                // south
                if (row + 1 < MAP_HEIGHT && MAP[row + 1][col] == 0)
                    addQuad(verts,
                        glm::vec3(x,        -0.05f, z + 1.0f),
                        glm::vec3(x + 1.0f, -0.05f, z + 1.0f),
                        glm::vec3(x + 1.0f,  1.05f, z + 1.0f),
                        glm::vec3(x,         1.05f, z + 1.0f),
                        wallNS);

                // north
                if (row - 1 >= 0 && MAP[row - 1][col] == 0)
                    addQuad(verts,
                        glm::vec3(x + 1.0f, -0.05f, z),
                        glm::vec3(x,        -0.05f, z),
                        glm::vec3(x,         1.05f, z),
                        glm::vec3(x + 1.0f,  1.05f, z),
                        wallNS);

                // east
                if (col + 1 < MAP_WIDTH && MAP[row][col + 1] == 0)
                    addQuad(verts,
                        glm::vec3(x + 1.0f, -0.05f, z),
                        glm::vec3(x + 1.0f, -0.05f, z + 1.0f),
                        glm::vec3(x + 1.0f,  1.05f, z + 1.0f),
                        glm::vec3(x + 1.0f,  1.05f, z),
                        wallEW);

                // west
                if (col - 1 >= 0 && MAP[row][col - 1] == 0)
                    addQuad(verts,
                        glm::vec3(x, -0.05f, z + 1.0f),
                        glm::vec3(x, -0.05f, z),
                        glm::vec3(x,  1.05f, z),
                        glm::vec3(x,  1.05f, z + 1.0f),
                        wallEW);
            }
        }
    }

    // upload to GPU
    m_VertexCount = verts.size() / 6;
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Level::draw(Shader &shader) {
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("uModel", model);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
    glBindVertexArray(0);
}
