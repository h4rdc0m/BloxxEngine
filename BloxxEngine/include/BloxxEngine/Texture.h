/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "glad/gl.h"

#include <string>

namespace BloxxEngine
{

class Texture
{
  public:

    enum class FilterMode
    {
        Nearest,
        Linear,
    };

    enum class WrapMode
    {
        Clamp,
        Repeat,
    };

    Texture(const std::string &filePath, FilterMode filterMode = FilterMode::Linear, WrapMode wrapMode = WrapMode::Clamp);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind();

    [[nodiscard]] inline int GetWidth() const
    {
        return m_Width;
    }
    [[nodiscard]] inline int GetHeight() const
    {
        return m_Height;
    }

  private:
    GLuint m_RendererID;
    std::string m_FilePath;
    unsigned char *m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
};

} // namespace BloxxEngine
