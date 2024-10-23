/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "World/Block.h"

#include <array>
#include <glm/vec2.hpp>
#include <unordered_map>

namespace BloxxEngine {

class TextureAtlas {
public:
    TextureAtlas(float textureSize, float atlasSize);
    std::array<glm::vec2, 4> GetUVs(std::string blockID, BlockFace::Direction faceDirection);

    private:
    float m_TextureSize;
    float m_AtlasSize;

    std::unordered_map<std::string, std::array<glm::vec2, 4>> m_TextureCoordinates;
};

} // BloxxEngine
