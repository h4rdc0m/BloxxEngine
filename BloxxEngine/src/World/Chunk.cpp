

/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/World/Chunk.h"

#include <iostream>
namespace BloxxEngine
{
Chunk::Chunk(const int x, const int z) : m_ChunkX(x), m_ChunkZ(z), m_VAO(0), m_VBO(0), m_EBO(0)
{
    auto* b = new Block{"block:stone", BlockType::Solid, 0};
    m_Blocks.fill(b);
}

void Chunk::GenerateMesh()
{
    m_Vertices.clear();
    m_Indices.clear();

    int m_IndexCount = 0;
    auto texCoords = std::array<glm::vec2, 4>{
        glm::vec2(0.0f, 1.0f), // Bottom-left
        glm::vec2(1.0f, 1.0f), // Bottom-right
        glm::vec2(1.0f, 0.0f), // Top-right
        glm::vec2(0.0f, 0.0f), // Top-left
    };

    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        const int x = i % CHUNK_WIDTH;
        const int y = (i / CHUNK_WIDTH) % CHUNK_HEIGHT;
        const int z = i / (CHUNK_WIDTH * CHUNK_HEIGHT);

        Block* block = m_Blocks.at(i);
        if (block == nullptr || block->Type == BlockType::Air)
        {
            continue;
        }

        // Positions in world space
        glm::vec3 blockPosition = glm::vec3(m_ChunkX * CHUNK_WIDTH + x, y, m_ChunkZ * CHUNK_DEPTH + z);

        // For each face, check if the adjacent block is air or transparent
        // If so, add the face to the mesh.

        // Top face
        if (y == CHUNK_HEIGHT - 1 || GetBlock(x, y + 1, z)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::TopVertices, BlockFace::TopNormal,texCoords);
        }

        // Bottom face
        if (y == 0 || GetBlock(x, y - 1, z)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::BottomVertices, BlockFace::BottomNormal, texCoords);
        }

        // Front face
        if (z == 0 || GetBlock(x, y, z - 1)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::BackVertices, BlockFace::BackNormal, texCoords);
        }

        // Back face
        if (z == CHUNK_DEPTH - 1 || GetBlock(x, y, z + 1)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::FrontVertices, BlockFace::FrontNormal, texCoords);
        }

        // Left face
        if (x == 0 || GetBlock(x - 1, y, z)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::LeftVertices, BlockFace::LeftNormal, texCoords);
        }

        // Right face
        if (x == CHUNK_WIDTH - 1 || GetBlock(x + 1, y, z)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::RightVertices, BlockFace::RightNormal, texCoords);
        }
    }
}
void Chunk::Draw() const
{
}

std::vector<Vertex> &Chunk::GetVertices()
{
    return m_Vertices;
}
std::vector<GLuint> &Chunk::GetIndices()
{
    return m_Indices;
}

void Chunk::SetupMesh()
{
}
void Chunk::AddFace(const glm::vec3 &blockPosition, const std::array<glm::vec3, 4> &faceVertices,
                    const glm::vec3 &faceNormal, const std::array<glm::vec2, 4> &uvCoords)
{
    // Each face has 4 vertices
    Vertex v0, v1, v2, v3;

    // Calculate positions of the vertices by adding the block position
    v0.Position = blockPosition + faceVertices[0];
    v1.Position = blockPosition + faceVertices[1];
    v2.Position = blockPosition + faceVertices[2];
    v3.Position = blockPosition + faceVertices[3];

    // Set normals
    v0.Normal = faceNormal;
    v1.Normal = faceNormal;
    v2.Normal = faceNormal;
    v3.Normal = faceNormal;

    // Set texture coordinates
    v0.TexCoords = uvCoords[0];
    v1.TexCoords = uvCoords[1];
    v2.TexCoords = uvCoords[2];
    v3.TexCoords = uvCoords[3];

    // Add the vertices to the mesh
    m_Vertices.push_back(v0);
    m_Vertices.push_back(v1);
    m_Vertices.push_back(v2);
    m_Vertices.push_back(v3);

    // Calculate the starting index for the indices
    GLuint index = static_cast<GLuint>(m_Vertices.size()) - 4;
    m_Indices.push_back(index);
    m_Indices.push_back(index + 1);
    m_Indices.push_back(index + 2);

    m_Indices.push_back(index + 2);
    m_Indices.push_back(index + 3);
    m_Indices.push_back(index);
}

Block *Chunk::GetBlock(const int x, const int y, const int z) const
{
    // Ensure x, y, z are within valid ranges
    assert(x >= 0 && x < CHUNK_WIDTH);
    assert(y >= 0 && y < CHUNK_HEIGHT);
    assert(z >= 0 && z < CHUNK_DEPTH);

    const int i = x + y * CHUNK_WIDTH + z * CHUNK_WIDTH * CHUNK_HEIGHT;
    return m_Blocks.at(i);
}
void Chunk::SetBlock(const int x, const int y, const int z, const std::string &id, const BlockType type,
                     const uint8_t metadata)
{
    const int i = x + y * CHUNK_WIDTH + z * CHUNK_WIDTH * CHUNK_HEIGHT;
    std::cout << "Adding block " << id << ", to: " << i << std::endl;

    m_Blocks[i] = new Block(id, type, metadata);
}

} // namespace BloxxEngine
