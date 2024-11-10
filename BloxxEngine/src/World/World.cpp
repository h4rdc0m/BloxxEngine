

/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/World/World.h"

#include <FastNoise/FastNoise.h>
#include <taskflow/taskflow.hpp>

namespace BloxxEngine
{
World::World()
{
    // Init the world with some initial chunks
    for (int x = -1; x <= 1; x++)
    {
        for (int z = -1; z <= 1; z++)
        {
            AddChunk(x, z);
        }
    }
}

World::~World() = default;

void World::Update(float deltaTime)
{
    // TODO: Tick the world and chunks
}

void World::Draw()
{
    std::lock_guard<std::mutex> lock(m_ChunkMeshesMutex);
    for (const auto &[position, mesh] : m_ChunkMeshes)
    {
        mesh->Draw();
    }
}

std::shared_ptr<Chunk> World::GetChunk(int x, int z) const
{
    if (const auto it = m_Chunks.find({x, z}); it != m_Chunks.end())
    {
        return it->second;
    }
    return nullptr;
}

void World::AddChunk(int x, int z)
{
    if (GetChunk(x, z) != nullptr)
        return;

    // Create a new chunk and initialize its blocks
    auto chunk = std::make_shared<Chunk>(x, z);

    m_Executor.async([this, chunk = std::move(chunk), x, z]() mutable {
        chunk->GenerateTerrain(x, z, WORLD_SEED);

        // Mark chunk as ready
        chunk->SetReady(true);

        // Add the chunk to the world
        {
            std::lock_guard<std::mutex> lock(m_ChunksMutex);
            m_Chunks[{x, z}] = chunk;
        }
    });

    m_Executor.async([this]() mutable { GenerateMeshes(); });
}
void World::RemoveChunk(int x, int z)
{
    m_Chunks.erase({x, z});
}

Block *World::GetBlock(const int x, const int y, const int z) const
{
    const int chunkX = x / CHUNK_WIDTH;
    const int chunkZ = z / CHUNK_DEPTH;
    if (const auto chunk = GetChunk(chunkX, chunkZ))
    {
        return chunk->GetBlock(x % CHUNK_WIDTH, y, z % CHUNK_DEPTH);
    }
    return nullptr;
}

void World::SetBlock(const int x, const int y, const int z, Block *id, const BlockType type,
                     const uint8_t metadata) const
{
    const int chunkX = x / CHUNK_WIDTH;
    const int chunkZ = z / CHUNK_DEPTH;
    if (const auto chunk = GetChunk(chunkX, chunkZ))
    {
        chunk->SetBlock(x % CHUNK_WIDTH, y, z % CHUNK_DEPTH, "block:stone", type, metadata);
    }
}
void World::GenerateMeshes()
{
    std::lock_guard<std::mutex> lock(m_ChunkMeshesMutex);

    for (const auto &[position, chunk] : m_Chunks)
    {
        if (chunk->IsReady() && !m_ChunkMeshes.contains(position))
        {
            // Generate mesh data
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            // Access chunk block data
            const ChunkBlockData &chunkBlocks = chunk->GetBlockData();

            std::shared_ptr<Chunk> lChunk = GetChunk(chunk->GetChunkX() - 1, chunk->GetChunkZ());
            std::shared_ptr<Chunk> rChunk = GetChunk(chunk->GetChunkX() + 1, chunk->GetChunkZ());
            std::shared_ptr<Chunk> fChunk = GetChunk(chunk->GetChunkX(), chunk->GetChunkZ() + 1);
            std::shared_ptr<Chunk> bChunk = GetChunk(chunk->GetChunkX(), chunk->GetChunkZ() - 1);

            // Generate optimized vertices and indices for the chunk, we're using CCW triangles
            GenerateChunkMesh(chunk, lChunk, rChunk, fChunk, bChunk, vertices, indices);
        }
    }
}
void World::GenerateChunkMesh(const std::shared_ptr<Chunk> &chunk, const std::shared_ptr<Chunk> &leftChunk,
                              const std::shared_ptr<Chunk> &rightChunk, const std::shared_ptr<Chunk> &frontChunk,
                              const std::shared_ptr<Chunk> &backChunk, std::vector<Vertex> &vertices,
                              std::vector<uint32_t> &indices)
{

    // Greedy meshing algorithm
    for (size_t blockFace = 0; blockFace < 6; ++blockFace)
    {
        glm::vec3 u{0, 0, 0};
        glm::vec3 v{0, 0, 0};

        switch (blockFace)
        {
        case 0:
            v = {1, 0, 0};
            u = {0, 0, 1};
            break; // Back Face
        case 1:
            v = {1, 0, 0};
            u = {0, 0, 1};
            break; // Front Face
        case 2:
            v = {0, 1, 0};
            u = {1, 0, 0};
            break; // Bottom Face
        case 3:
            v = {0, 1, 0};
            u = {1, 0, 0};
            break; // Top Face
        case 4:
            v = {1, 0, 0};
            u = {0, 1, 0};
            break; // Left Face
        case 5:
            v = {1, 0, 0};
            u = {0, 1, 0};
            break; // Right Face
        }

        for (int d3 = 0; d3 < CHUNK_DEPTH; ++d3)
        {
            for (int d2 = 0; d2 < CHUNK_HEIGHT; ++d2)
            {
                for (int d1 = 0; d1 < CHUNK_WIDTH; ++d1)
                {
                    // Check if current block is solid

                    if (const Block *block = chunk->GetBlock(d1, d2, d3); block != nullptr && block->IsSolid())
                    {
                        // Check adjacent blocks to see if face should be culled
                        bool shouldCull = false;
                        switch (blockFace)
                        {
                        case 0:
                            if (d3 == 0)
                                shouldCull = leftChunk && leftChunk->GetBlock(d1, d2, CHUNK_DEPTH - 1)->IsSolid();
                            else
                                shouldCull = chunk->GetBlock(d1, d2, d3 - 1)->IsSolid();
                            break;
                        case 1:
                            if (d3 == CHUNK_DEPTH - 1)
                                shouldCull = rightChunk && rightChunk->GetBlock(d1, d2, 0)->IsSolid();
                            else
                                shouldCull = chunk->GetBlock(d1, d2, d3 + 1)->IsSolid();
                            break;
                        case 2:
                            if (d2 == 0)
                                shouldCull = backChunk && backChunk->GetBlock(d1, CHUNK_HEIGHT - 1, d3)->IsSolid();
                            else
                                shouldCull = chunk->GetBlock(d1, d2 - 1, d3)->IsSolid();
                            break;
                        case 3:
                            if (d2 == CHUNK_HEIGHT - 1)
                                shouldCull = frontChunk && frontChunk->GetBlock(d1, 0, d3)->IsSolid();
                            else
                                shouldCull = chunk->GetBlock(d1, d2 + 1, d3)->IsSolid();
                            break;
                        case 4:
                            if (d1 == 0)
                                shouldCull = leftChunk && leftChunk->GetBlock(CHUNK_WIDTH - 1, d2, d3)->IsSolid();
                            else
                                shouldCull = chunk->GetBlock(d1 - 1, d2, d3)->IsSolid();
                            break;
                        case 5:
                            if (d1 == CHUNK_WIDTH - 1)
                                shouldCull = rightChunk && rightChunk->GetBlock(0, d2, d3)->IsSolid();
                            else
                                shouldCull = chunk->GetBlock(d1 + 1, d2, d3)->IsSolid();
                            break;
                        default:
                            shouldCull = false;
                        }

                        // If face should not be culled, add it to the mesh
                        if (!shouldCull)
                        {
                            // Add vertices and indices for this face
                            glm::vec3 position(d1, d2, d3);
                            glm::vec3 normal;
                            switch (blockFace)
                            {
                            case 0:
                                normal = glm::vec3(0.0f, 0.0f, -1.0f);
                                break;
                            case 1:
                                normal = glm::vec3(0.0f, 0.0f, 1.0f);
                                break;
                            case 2:
                                normal = glm::vec3(0.0f, -1.0f, 0.0f);
                                break;
                            case 3:
                                normal = glm::vec3(0.0f, 1.0f, 0.0f);
                                break;
                            case 4:
                                normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                                break;
                            case 5:
                                normal = glm::vec3(1.0f, 0.0f, 0.0f);
                                break;
                            }

                            std::array<glm::vec3, 4> faceVertices{glm::vec3(position + u), glm::vec3(position + u + v),
                                                                  glm::vec3(position + v), glm::vec3(position)};

                            size_t indexStart = vertices.size();
                            for (const auto &vertexPos : faceVertices)
                            {
                                vertices.emplace_back(Vertex{vertexPos, normal, glm::vec2(0.0f)});
                            }

                            indices.push_back(static_cast<uint32_t>(indexStart + 0));
                            indices.push_back(static_cast<uint32_t>(indexStart + 1));
                            indices.push_back(static_cast<uint32_t>(indexStart + 2));
                            indices.push_back(static_cast<uint32_t>(indexStart + 2));
                            indices.push_back(static_cast<uint32_t>(indexStart + 3));
                            indices.push_back(static_cast<uint32_t>(indexStart + 0));
                        }
                    }
                }
            }
        }
    }
}
void World::SetupMeshData()
{
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    m_ChunkMeshes[{chunk->GetChunkX(), chunk->GetChunkZ()}] = mesh;
}
} // namespace BloxxEngine