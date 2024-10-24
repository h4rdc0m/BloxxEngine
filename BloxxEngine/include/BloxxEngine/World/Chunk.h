/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "Block.h"
#include "BloxxEngine/Mesh.h"

#include <array>
#include <vector>

namespace BloxxEngine
{

constexpr int CHUNK_WIDTH = 2;
constexpr int CHUNK_HEIGHT = 2;
constexpr int CHUNK_DEPTH = 2;
constexpr int CHUNK_SIZE = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

class Chunk
{
  public:
    Chunk(int x, int z);
    ~Chunk() = default;

    void GenerateMesh();
    void Draw() const;

    std::vector<Vertex>& GetVertices();
    std::vector<GLuint>& GetIndices();

    // Accessor for blocks
    [[nodiscard]] Block *GetBlock(int x, int y, int z) const;
    void SetBlock(int x, int y, int z, const std::string &id, BlockType type, uint8_t metadata = 0);

    [[nodiscard]] int GetChunkX() const
    {
        return m_ChunkX;
    }
    [[nodiscard]] int GetChunkZ() const
    {
        return m_ChunkZ;
    }

  private:
    int m_ChunkX, m_ChunkZ;

    std::array<Block *, CHUNK_SIZE> m_Blocks{};

    // Mesh data
    std::vector<Vertex> m_Vertices;
    std::vector<GLuint> m_Indices;
    GLuint m_VAO{}, m_VBO{}, m_EBO{};

    void SetupMesh();
    void AddFace(const glm::vec3 &blockPosition, const std::array<glm::vec3, 4> &faceVertices,
                 const glm::vec3 &faceNormal, const std::array<glm::vec2, 4> &uvCoords);
};
} // namespace BloxxEngine