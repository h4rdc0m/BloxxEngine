/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include <glm/glm.hpp>

#include <vector>

namespace BloxxEngine
{

#pragma pack(push, 1)
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal = {0.0f, 0.0f, 1.0f};
    glm::vec2 TexCoords;
    glm::vec3 Tangent = {.0f, .0f, .0f}; // Tangent attribute
    glm::vec3 Bitangent = {.0f, .0f, .0f};
    ; // Bitangent attribute
};
#pragma pack(pop)

class Mesh
{
  public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
    ~Mesh();

    void Draw() const;
    void CalculateTangentsAndBitangents();

  private:
    uint32_t m_VAO, m_VBO, m_EBO;
    size_t m_IndexCount;

    void SetupMesh();
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
};

} // namespace BloxxEngine
