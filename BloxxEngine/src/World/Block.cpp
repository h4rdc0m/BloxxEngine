

/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/World/Block.h"

namespace BloxxEngine
{

Block::Block(const std::string& ID, const BlockType type, const uint8_t Metadata) : ID(ID), Type(type), Metadata(Metadata)
{
}

bool Block::IsSolid() const
{
    switch (Type)
    {
    case BlockType::Solid:
        return true;
    default:
        return false;
    }
}

bool Block::IsTransparent() const
{
    switch (Type)
    {
    case BlockType::Solid:
        return false;
    default:
        return true;
    }
}

} // namespace BloxxEngine