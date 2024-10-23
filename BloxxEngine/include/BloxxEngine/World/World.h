/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "Chunk.h"

#include <memory>
#include <unordered_map>

namespace BloxxEngine
{

class World
{
  public:
    World();
    ~World();

    void Update(float deltaTime);
    void Draw();

    Chunk *GetChunk(int x, int y, int z);
    void AddChunk(int x, int z);
    void RemoveChunk(int x, int z);

    // Coordinate conversion functions
    [[nodiscard]] Block *GetBlock(int x, int y, int z) const;
    void SetBlock(int x, int y, int z, Block *id, BlockType type, uint8_t metadata);

  private:
    struct PairHash
    {
        std::size_t operator()(const std::pair<int, int> &p) const
        {
            return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
        }
    };

    // Map chunk positions to chunk pointers
    std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, PairHash> m_Chunks;
};

} // namespace BloxxEngine
