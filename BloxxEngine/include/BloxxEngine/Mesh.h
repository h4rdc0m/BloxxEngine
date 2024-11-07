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
};
#pragma pack(pop)

class Mesh
{
  public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
    ~Mesh();

    void Draw() const;

  private:
    void SetupMesh();

    uint32_t m_VAO = 0;
    uint32_t m_VBO = 0;
    uint32_t m_EBO = 0;
    size_t m_IndexCount = 0;

    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
};

} // namespace BloxxEngine
