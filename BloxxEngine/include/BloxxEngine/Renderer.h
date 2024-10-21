/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <GLFW/glfw3.h>
#include <memory>

#include <glm/glm.hpp>

#include <string>

namespace BloxxEngine
{

class Renderer
{
  public:
    Renderer();
    virtual ~Renderer();

    bool Initialize(const std::string &windowTitle, int width, int height);
    void Run();
    void Shutdown();

  protected:
    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();
    virtual void OnImGuiRender(); // For future ImGUI integration

  private:
    bool InitializeGLFW();
    static bool InitializeGlad();
    void MainLoop();

    void ProcessInput() const;

    // GLFW window
    GLFWwindow *m_Window;

    // Shader, Texture, and Mesh
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;
    std::unique_ptr<Mesh> m_Mesh;

    // Timing
    float m_LastFrameTime;

    // MVP matrices
    glm::mat4 m_ModelMatrix{0};
    glm::mat4 m_ViewMatrix{0};
    glm::mat4 m_ProjectionMatrix{0};

    // Window properties
    std::string m_WindowTitle;
    int m_Width, m_Height;
};

} // namespace BloxxEngine
