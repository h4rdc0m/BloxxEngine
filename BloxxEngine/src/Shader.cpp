/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/Shader.h"

#include <fstream>
#include <iostream>
#include <ostream>

namespace BloxxEngine
{
Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    const auto vertexShaderSrc = LoadShaderSource(vertexShaderPath);
    const auto fragmentShaderSrc = LoadShaderSource(fragmentShaderPath);
    
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    m_RendererID = CreateProgram(vertexShader, fragmentShader);

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}
void Shader::SetUniformMat4(const std::string &name, const glm::mat4 &value)
{
    const GLint location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniformInt(const std::string &name, const int value)
{
    const GLint location = GetUniformLocation(name);
    glUniform1i(location, value);
}
void Shader::SetUniformFloat(const std::string &name, const float value)
{
    const GLint location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void Shader::SetUniformVec3(const std::string &name, const glm::vec3 &value)
{
    const GLint location = GetUniformLocation(name);
    glUniform3f(location, value.x, value.y, value.z);
}


std::string Shader::LoadShaderSource(const std::string &path)
{

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Warning: Could not open file at " << path << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

GLuint Shader::CompileShader(const GLenum type, const std::string &source)
{
    const GLuint shader = glCreateShader(type);
    const GLchar *shaderSource = source.c_str();
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Error handling
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
        std::cerr << "Shader compilation failed:\n" << &infoLog[0] << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
GLuint Shader::CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Error handling
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        std::cerr << "Program link failed:\n" << &infoLog[0] << std::endl;
        glDeleteProgram(program);
        return 0;
    }

    return program;
}
GLint Shader::GetUniformLocation(const std::string &name)
{
    // Check if the location is already in cache
    if (m_UniformLocationCache.contains(name))
        return m_UniformLocationCache[name];

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cerr << "Uniform " << name << " not found" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

} // namespace BloxxEngine