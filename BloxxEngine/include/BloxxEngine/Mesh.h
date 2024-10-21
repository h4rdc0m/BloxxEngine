/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <vector>

namespace BloxxEngine {

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal = {0.0f, 0.0f, 1.0f};
    glm::vec2 TexCoords;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    ~Mesh();

    void Draw() const;

    private:
    GLuint m_VAO, m_VBO, m_EBO;
    size_t m_IndexCount;

    void SetupMesh();
    std::vector<Vertex> m_Vertices;
    std::vector<GLuint> m_Indices;
};

} // BloxxEngine
