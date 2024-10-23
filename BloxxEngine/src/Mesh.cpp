/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/Mesh.h"

#include <iostream>

namespace BloxxEngine
{
Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
    : m_VAO(0), m_VBO(0), m_EBO(0), m_IndexCount(indices.size()), m_Vertices(vertices), m_Indices(indices)
{
    CalculateTangentsAndBitangents();
    SetupMesh();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Mesh::SetupMesh()
{
    std::cout << "Size of Vertex: " << sizeof(Vertex) << std::endl;
    std::cout << "Offset of Position: " << offsetof(Vertex, Position) << std::endl;
    std::cout << "Offset of Normal: " << offsetof(Vertex, Normal) << std::endl;
    std::cout << "Offset of TexCoords: " << offsetof(Vertex, TexCoords) << std::endl;
    std::cout << "Offset of Tangent: " << offsetof(Vertex, Tangent) << std::endl;
    std::cout << "Offset of Bitangent: " << offsetof(Vertex, Bitangent) << std::endl;

    // Generate buffers and arrays
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    // Bind VAO
    glBindVertexArray(m_VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

    // Element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    // Position (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
    // Normal (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // Texture coordinates (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    // Tangent (location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    // Bitangent (location = 4)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));


    // Unbind VAO
    glBindVertexArray(0);

    std::cout << "Index Count: " << m_IndexCount << std::endl;
}

void Mesh::Draw() const
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
void Mesh::CalculateTangentsAndBitangents() {
   // Iterate over each triangle
    for (size_t i = 0; i < m_Indices.size(); i += 3) {
        Vertex& v0 = m_Vertices[m_Indices[i]];
        Vertex& v1 = m_Vertices[m_Indices[i + 1]];
        Vertex& v2 = m_Vertices[m_Indices[i + 2]];

        // Positions
        glm::vec3& p0 = v0.Position;
        glm::vec3& p1 = v1.Position;
        glm::vec3& p2 = v2.Position;

        // Texture coordinates
        glm::vec2& uv0 = v0.TexCoords;
        glm::vec2& uv1 = v1.TexCoords;
        glm::vec2& uv2 = v2.TexCoords;

        // Edges of the triangle : position delta
        glm::vec3 deltaPos1 = p1 - p0;
        glm::vec3 deltaPos2 = p2 - p0;

        // UV delta
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        // Accumulate tangents and bitangents
        v0.Tangent += tangent;
        v1.Tangent += tangent;
        v2.Tangent += tangent;

        v0.Bitangent += bitangent;
        v1.Bitangent += bitangent;
        v2.Bitangent += bitangent;

    }

    // Normalize the tangents and bitangents
    for (auto& vertex : m_Vertices) {
        vertex.Tangent = glm::normalize(vertex.Tangent);
        vertex.Bitangent = glm::normalize(vertex.Bitangent);
    }
}

} // namespace BloxxEngine