#include "mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <iostream>

Mesh::Mesh() {}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    if (m_texture) glDeleteTextures(1, &m_texture);
}

bool Mesh::loadTexture(const std::string& path)
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "STB failed to load: " << path << "\n";
        return false;
    }
    std::cout << "Texture loaded: " << width << "x" << height << " ch:" << channels << "\n";

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return true;
}

bool Mesh::load(const std::string& objPath, const std::string& texturePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    // get directory of the obj file for mtl lookup
    std::string baseDir = objPath.substr(0, objPath.find_last_of("/\\") + 1);

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str(), baseDir.c_str())) {
        std::cerr << "Failed to load OBJ: " << err << "\n";
        return false;
    }

    std::vector<float> verts;
    for (auto& shape : shapes) {
        for (auto& index : shape.mesh.indices) {
            // position
            verts.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            verts.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            verts.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            // UV
            if (index.texcoord_index >= 0) {
                verts.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                verts.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            } else {
                verts.push_back(0.0f);
                verts.push_back(0.0f);
            }
        }
    }

    // position(3) + uv(2) = 5 floats per vertex
    m_vertexCount = verts.size() / 5;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    // position - location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // uv - location 1
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    if (!texturePath.empty())
        loadTexture(texturePath);

    return true;
}

void Mesh::draw(Shader& shader)
{
    if (m_texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        shader.setInt("uTexture", 0);
        shader.setBool("uUseTexture", true);
    } else {
        shader.setBool("uUseTexture", false);
    }

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}