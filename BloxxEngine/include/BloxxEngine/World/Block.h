

/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "BloxxEngine/Mesh.h"

#include <array>
#include <cstdint>
#include <glm/vec3.hpp>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

namespace BloxxEngine
{

enum class BlockType : uint8_t
{
    Solid = 0,
    Water,
    Air,
    Entity,
};

namespace BlockFace
{
enum class Direction
{
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom
};

// Front Face (+Z) - CCW Order when viewed from +Z
constexpr std::array<glm::vec3, 4> FrontVertices = {
    glm::vec3(0.0f, 0.0f, 1.0f), // Bottom-left
    glm::vec3(1.0f, 0.0f, 1.0f), // Bottom-right
    glm::vec3(1.0f, 1.0f, 1.0f), // Top-right
    glm::vec3(0.0f, 1.0f, 1.0f)  // Top-left
};
constexpr glm::vec3 FrontNormal = glm::vec3(0.0f, 0.0f, 1.0f);

// Back Face (-Z) - CCW Order when viewed from -Z
constexpr std::array<glm::vec3, 4> BackVertices = {
    glm::vec3(1.0f, 0.0f, 0.0f), // Bottom-right
    glm::vec3(0.0f, 0.0f, 0.0f), // Bottom-left
    glm::vec3(0.0f, 1.0f, 0.0f), // Top-left
    glm::vec3(1.0f, 1.0f, 0.0f)  // Top-right
};
constexpr glm::vec3 BackNormal = glm::vec3(0.0f, 0.0f, -1.0f);

// Left Face (-X) - CCW Order when viewed from -X
constexpr std::array<glm::vec3, 4> LeftVertices = {
    glm::vec3(0.0f, 0.0f, 1.0f), // Bottom-left
    glm::vec3(0.0f, 1.0f, 1.0f), // Bottom-right
    glm::vec3(0.0f, 1.0f, 0.0f), // Top-right
    glm::vec3(0.0f, 0.0f, 0.0f)  // Top-left
};
constexpr glm::vec3 LeftNormal = glm::vec3(-1.0f, 0.0f, 0.0f); // Corrected Normal

// Right Face (+X) - CCW Order when viewed from +X
constexpr std::array<glm::vec3, 4> RightVertices = {
    glm::vec3(1.0f, 0.0f, 0.0f), // Bottom-left
    glm::vec3(1.0f, 1.0f, 0.0f), // Bottom-right
    glm::vec3(1.0f, 1.0f, 1.0f), // Top-right
    glm::vec3(1.0f, 0.0f, 1.0f)  // Top-left
};
constexpr glm::vec3 RightNormal = glm::vec3(1.0f, 0.0f, 0.0f); // Corrected Normal

// Top Face (+Y) - Corrected to ensure the face is not inset
// This face is on the top (Y=1), so all vertices should have Y=1.
constexpr std::array<glm::vec3, 4> TopVertices = {
    glm::vec3(0.0f, 1.0f, 1.0f), // Bottom-left (z = 1)
    glm::vec3(1.0f, 1.0f, 1.0f), // Bottom-right (z = 1)
    glm::vec3(1.0f, 1.0f, 0.0f), // Top-right (z = 0)
    glm::vec3(0.0f, 1.0f, 0.0f)  // Top-left (z = 0)
};
constexpr glm::vec3 TopNormal = glm::vec3(0.0f, 1.0f, 0.0f);

// Bottom Face (-Y) - Corrected to ensure the face is not inset
// This face is on the bottom (Y=0), so all vertices should have Y=0.
constexpr std::array<glm::vec3, 4> BottomVertices = {
    glm::vec3(0.0f, 0.0f, 0.0f), // Top-left
    glm::vec3(1.0f, 0.0f, 0.0f), // Top-right
    glm::vec3(1.0f, 0.0f, 1.0f), // Bottom-right
    glm::vec3(0.0f, 0.0f, 1.0f)  // Bottom-left
};
constexpr glm::vec3 BottomNormal = glm::vec3(0.0f, -1.0f, 0.0f);
}; // namespace BlockFace

struct BlockTexture
{
    std::string Top;
    std::string Bottom;
    std::string Left;
    std::string Right;
    std::string Front;
    std::string Back;
};

/**
 * This struct holds the texture names to load from the texture atlas.
 */
struct BlockTextures
{
    std::unique_ptr<BlockTexture> BaseColor;
    std::unique_ptr<BlockTexture> Normal;
    std::unique_ptr<BlockTexture> Roughness;
    std::unique_ptr<BlockTexture> Metallic;
};

struct Block
{
    std::string ID; // TODO: maybe change to string label
    BlockType Type;

    // Metadata can include additional properties, like block states
    uint8_t Metadata;

    std::string Name;
    std::unique_ptr<BlockTextures> Textures;

    explicit Block(const std::string &ID, BlockType type = BlockType::Air, uint8_t Metadata = 0);

    // Helper functions to check if block is solid, transparent etc.
    [[nodiscard]] bool IsSolid() const;
    [[nodiscard]] bool IsTransparent() const;
};

} // namespace BloxxEngine
