/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace BloxxEngine
{

class Shader
{
  public:
    Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    ~Shader();

    void Bind() const;
    void Unbind();

    // Utility functions to set uniform variables
    void SetUniformMat4(const std::string &name, const glm::mat4 &value);
    void SetUniformInt(const std::string &name, int value);
    void SetUniformFloat(const std::string &name, float value);
    void SetUniformVec3(const std::string &name, const glm::vec3 &value);

  private:
    GLuint m_RendererID;

    // Helper functions
    std::string LoadShaderSource(const std::string &path);
    GLuint CompileShader(GLenum type, const std::string &source);
    GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);
    GLint GetUniformLocation(const std::string &name);

    // Cache for uniform locations
    std::unordered_map<std::string, GLint> m_UniformLocationCache;
};

} // namespace BloxxEngine
