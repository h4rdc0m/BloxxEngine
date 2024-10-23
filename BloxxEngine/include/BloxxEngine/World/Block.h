

/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include <array>
#include <cstdint>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace BloxxEngine {

enum class BlockType : uint8_t
{
    Solid = 0,
    Water,
    Air,
    Entity,
};



namespace BlockFace
{
enum class Direction {
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom
};
// Front Face (+Z)
constexpr std::array<glm::vec3, 4> FrontVertices = {
    glm::vec3(0.0f, 0.0f, 1.0f), // Bottom-left
    glm::vec3(1.0f, 0.0f, 1.0f), // Bottom-right
    glm::vec3(1.0f, 1.0f, 1.0f), // Top-right
    glm::vec3(0.0f, 1.0f, 1.0f)  // Top-left
};
constexpr auto FrontNormal = glm::vec3(0.0f, 0.0f, 1.0f);

// Back Face (-Z)
constexpr std::array BackVertices = {
    glm::vec3(1.0f, 0.0f, 0.0f), // Bottom-left
    glm::vec3(0.0f, 0.0f, 0.0f), // Bottom-right
    glm::vec3(0.0f, 1.0f, 0.0f), // Top-right
    glm::vec3(1.0f, 1.0f, 0.0f)  // Top-left
};
constexpr glm::vec3 BackNormal = glm::vec3(0.0f, 0.0f, -1.0f);

// Left Face (-X)
constexpr std::array LeftVertices = {
    glm::vec3(0.0f, 0.0f, 0.0f), // Bottom-left
    glm::vec3(0.0f, 0.0f, 1.0f), // Bottom-right
    glm::vec3(0.0f, 1.0f, 1.0f), // Top-right
    glm::vec3(0.0f, 1.0f, 0.0f)  // Top-left
};
constexpr auto LeftNormal = glm::vec3(-1.0f, 0.0f, 0.0f);

// Right Face (+X)
constexpr std::array RightVertices = {
    glm::vec3(1.0f, 0.0f, 1.0f), // Bottom-left
    glm::vec3(1.0f, 0.0f, 0.0f), // Bottom-right
    glm::vec3(1.0f, 1.0f, 0.0f), // Top-right
    glm::vec3(1.0f, 1.0f, 1.0f)  // Top-left
};
constexpr auto RightNormal = glm::vec3(1.0f, 0.0f, 0.0f);

// Top Face (+Y)
constexpr std::array TopVertices = {
    glm::vec3(0.0f, 1.0f, 1.0f), // Bottom-left
    glm::vec3(1.0f, 1.0f, 1.0f), // Bottom-right
    glm::vec3(1.0f, 1.0f, 0.0f), // Top-right
    glm::vec3(0.0f, 1.0f, 0.0f)  // Top-left
};
constexpr auto TopNormal = glm::vec3(0.0f, 1.0f, 0.0f);

// Bottom Face (-Y)
constexpr std::array BottomVertices = {
    glm::vec3(0.0f, 0.0f, 0.0f), // Bottom-left
    glm::vec3(1.0f, 0.0f, 0.0f), // Bottom-right
    glm::vec3(1.0f, 0.0f, 1.0f), // Top-right
    glm::vec3(0.0f, 0.0f, 1.0f)  // Top-left
};
constexpr auto BottomNormal = glm::vec3(0.0f, -1.0f, 0.0f);
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

struct Block {
    std::string ID; // TODO: maybe change to string label
    BlockType Type;

    // Metadata can include additional properties, like block states
    uint8_t Metadata;

    std::string Name;
    std::unique_ptr<BlockTextures> Textures;

    explicit Block(const std::string& ID, BlockType type = BlockType::Air, uint8_t Metadata = 0);

    // Helper functions to check if block is solid, transparent etc.
    [[nodiscard]] bool IsSolid() const;
    [[nodiscard]] bool IsTransparent() const;
};

} // BloxxEngine
