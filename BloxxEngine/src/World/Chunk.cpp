

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
    auto *b = new Block{"block:stone", BlockType::Solid, 0};
    m_Blocks.fill(b);
}

Chunk::~Chunk()
{
    m_Blocks.fill(nullptr);
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

        // Position in world space
        glm::vec3 blockPosition = glm::vec3(m_ChunkX * CHUNK_WIDTH + x, y, m_ChunkZ * CHUNK_DEPTH + z);

        // Top face (+Y)
        if (y == CHUNK_HEIGHT - 1 || GetBlock(x, y + 1, z)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::TopVertices, BlockFace::TopNormal, {
                glm::vec2(0.0f, 0.0f), // Bottom-left
                glm::vec2(1.0f, 0.0f), // Bottom-right
                glm::vec2(1.0f, 1.0f), // Top-right
                glm::vec2(0.0f, 1.0f), // Top-left
            });
        }

        // Bottom face (-Y)
        if (y == 0 || GetBlock(x, y - 1, z)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::BottomVertices, BlockFace::BottomNormal, {
                glm::vec2(0.0f, 0.0f), // Bottom-left
                glm::vec2(1.0f, 0.0f), // Bottom-right
                glm::vec2(1.0f, 1.0f), // Top-right
                glm::vec2(0.0f, 1.0f), // Top-left
            });
        }

        // Front face (+Z)
        if (z == CHUNK_DEPTH - 1 || GetBlock(x, y, z + 1)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::FrontVertices, BlockFace::FrontNormal, {
                glm::vec2(0.0f, 0.0f), // Bottom-left
                glm::vec2(1.0f, 0.0f), // Bottom-right
                glm::vec2(1.0f, 1.0f), // Top-right
                glm::vec2(0.0f, 1.0f), // Top-left
            });
        }

        // Back face (-Z)
        if (z == 0 || GetBlock(x, y, z - 1)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::BackVertices, BlockFace::BackNormal, {
                glm::vec2(0.0f, 0.0f), // Bottom-left
                glm::vec2(1.0f, 0.0f), // Bottom-right
                glm::vec2(1.0f, 1.0f), // Top-right
                glm::vec2(0.0f, 1.0f), // Top-left
            });
        }

        // Left face (-X)
        if (x == 0 || GetBlock(x - 1, y, z)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::LeftVertices, BlockFace::LeftNormal, {
                glm::vec2(0.0f, 0.0f), // Bottom-left
                glm::vec2(1.0f, 0.0f), // Bottom-right
                glm::vec2(1.0f, 1.0f), // Top-right
                glm::vec2(0.0f, 1.0f), // Top-left
            });
        }

        // Right face (+X)
        if (x == CHUNK_WIDTH - 1 || GetBlock(x + 1, y, z)->IsTransparent())
        {
            AddFace(blockPosition, BlockFace::RightVertices, BlockFace::RightNormal, {
                glm::vec2(0.0f, 0.0f), // Bottom-left
                glm::vec2(1.0f, 0.0f), // Bottom-right
                glm::vec2(1.0f, 1.0f), // Top-right
                glm::vec2(0.0f, 1.0f), // Top-left
            });
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
std::vector<uint32_t> &Chunk::GetIndices()
{
    return m_Indices;
}

void Chunk::SetupMesh()
{
}
void Chunk::AddFace(
    const glm::vec3& blockPosition,
    const std::array<glm::vec3, 4>& faceVertices,
    const glm::vec3& normal,
    const std::array<glm::vec2, 4>& texCoords)
{
    unsigned int startIndex = static_cast<unsigned int>(m_Vertices.size());

    // Add vertices
    for (int i = 0; i < 4; ++i)
    {
        Vertex vertex;
        vertex.Position = blockPosition + faceVertices[i];
        vertex.Normal = normal;
        vertex.TexCoords = texCoords[i];
        // Calculate Tangent and Bitangent if using normal mapping
        m_Vertices.push_back(vertex);
    }

    // Add two triangles (CCW winding)
    m_Indices.push_back(startIndex + 0);
    m_Indices.push_back(startIndex + 1);
    m_Indices.push_back(startIndex + 2);

    m_Indices.push_back(startIndex + 2);
    m_Indices.push_back(startIndex + 3);
    m_Indices.push_back(startIndex + 0);
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
