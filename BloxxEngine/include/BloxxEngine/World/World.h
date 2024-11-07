/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "Chunk.h"

#include <chrono>
#include <memory>
#include <mutex>
#include <taskflow/core/executor.hpp>
#include <unordered_map>

namespace BloxxEngine
{

const int WORLD_SEED = static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count());

struct ChunkPairHash
{
    std::size_t operator()(const std::pair<int, int> &p) const
    {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
    }
};

template <typename T> using ChunkMap = std::unordered_map<std::pair<int, int>, std::shared_ptr<T>, ChunkPairHash>;
class World
{
  public:
    World();
    ~World();

    void Update(float deltaTime);
    void Draw();

    [[nodiscard]] std::shared_ptr<Chunk> GetChunk(int x, int z) const;
    void AddChunk(int x, int z);
    void RemoveChunk(int x, int z);

    // Coordinate conversion functions
    [[nodiscard]] Block *GetBlock(int x, int y, int z) const;
    void SetBlock(int x, int y, int z, Block *id, BlockType type, uint8_t metadata) const;

    [[nodiscard]] const ChunkMap<Chunk> &GetChunks() const
    {
        return m_Chunks;
    }

    std::mutex &GetChunksMutex() const
    {
        return m_ChunksMutex;
    }

  private:
    // Map of chunks
    ChunkMap<Chunk> m_Chunks;

    // Map of generated meshes for corresponding chunks
    ChunkMap<Mesh> m_ChunkMeshes;

    // Mutex for thread safety
    mutable std::mutex m_ChunksMutex;

    // Taskflow executor
    tf::Executor m_Executor;

    void GenerateMeshes();
    void GenerateChunkMesh(const std::shared_ptr<Chunk> &chunk, const std::shared_ptr<Chunk> &leftChunk,
                                  const std::shared_ptr<Chunk> &rightChunk, const std::shared_ptr<Chunk> &frontChunk,
                                  const std::shared_ptr<Chunk> &backChunk, std::vector<Vertex> &vertices,
                                  std::vector<uint32_t> &indices);
};

} // namespace BloxxEngine
