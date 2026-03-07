#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "core/Shader.h"

//0 = empty space, 1 = wall

const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

const int MAP[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0 ,0 ,0 ,1},
    {1 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,1 ,1},
    {1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1},
    {1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1}
};

class Level {
    public:
        Level();
        ~Level();
    // generate 3D geometry from the tilemap
    void build();

    //draw the level
    void draw(Shader& shader);

    private:
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        int m_VertexCount = 0;

    // add a single quad (2 triangles) to the vertex list
    void addQuad(std::vector<float>& verts,
        glm::vec3 p1, glm::vec3 p2,
        glm::vec3 p3, glm::vec3 p4,
        glm::vec3 color);

};