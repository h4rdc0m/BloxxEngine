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

constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_HEIGHT = 256;
constexpr int CHUNK_DEPTH = 16;
constexpr int CHUNK_SIZE = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

typedef std::array<Block *, CHUNK_SIZE> ChunkBlockData;

class Chunk
{
  public:
    Chunk(int x, int z);
    ~Chunk();

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

    [[nodiscard]] bool IsReady() const
    {
        return m_IsReady;
    }
    [[nodiscard]] const ChunkBlockData& GetBlockData() const
    {
        return m_Blocks;
    }

    void SetReady(const bool ready) { m_IsReady = ready; }

    void GenerateTerrain(int x, int z, int seed);

  private:
    int m_ChunkX, m_ChunkZ;
    bool m_IsReady = false;

    ChunkBlockData m_Blocks{};
};
} // namespace BloxxEngine