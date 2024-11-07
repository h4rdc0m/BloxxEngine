

/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/World/Chunk.h"

#include "BloxxEngine/Core/Log.h"
#include "FastNoise/FastNoise.h"

#include <glad/gl.h>
#include <iostream>

namespace BloxxEngine
{
Chunk::Chunk(const int x, const int z) : m_ChunkX(x), m_ChunkZ(z)
{
    auto *b = new Block{"block:stone", BlockType::Solid, 0};
    m_Blocks.fill(b);
}

Chunk::~Chunk()
{
    m_Blocks.fill(nullptr);
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

    //BE_CORE_INFO("Adding block {}, to: Chunk({}, {}), at: {},{},{}", id, m_ChunkX, m_ChunkZ,x,y,z);


    m_Blocks[i] = new Block(id, type, metadata);
}
void Chunk::GenerateTerrain(const int x, const int z, const int seed)
{
    auto simplex = FastNoise::New<FastNoise::Simplex>();
    for (uint8_t bx = 0; bx < CHUNK_WIDTH; bx++)
    {
        for (uint8_t bz = 0; bz < CHUNK_DEPTH; bz++)
        {
            const double height = simplex->GenSingle2D((x * CHUNK_WIDTH + bx)*0.1f, (z * CHUNK_DEPTH + bz)*0.1f, seed) * 20.0;
            for (int by = 0; by < height; by++)
            {
                SetBlock(bx, by, bz, "block:stone", BlockType::Solid);
            }
        }
    }
}

} // namespace BloxxEngine
