/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/TextureAtlas.h"

namespace BloxxEngine {
TextureAtlas::TextureAtlas(float textureSize, float atlasSize) : m_TextureSize(textureSize), m_AtlasSize(atlasSize)
{
    m_TextureCoordinates = {};
}
} // BloxxEngine