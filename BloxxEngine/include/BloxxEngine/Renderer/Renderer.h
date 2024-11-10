/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once

#define GLFW_INCLUDE_NONE
#include "BloxxEngine/Camera.h"
#include "BloxxEngine/Mesh.h"
#include "BloxxEngine/Shader.h"
#include "BloxxEngine/Texture.h"

#include <GLFW/glfw3.h>
#include <memory>

#include <glm/glm.hpp>

#include <string>

namespace BloxxEngine
{
class World;

class Renderer final
{
  public:
    Renderer(const std::shared_ptr<Camera>& camera, int width, int height);
    virtual ~Renderer();

    bool Init();
    void Render(World &world);
    void Update(float deltaTime);
    void Shutdown();

    void SetProjectionMatrix(const glm::mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }

    [[nodiscard]] bool GetRenderWireframe() const
    {
        return m_RenderWireframe;
    }
    void SetRenderWireframe(const bool m_render_wireframe)
    {
        m_RenderWireframe = m_render_wireframe;
    }

  private:
    void ImGuiRender();
    float m_DeltaTime = 0.0f;
    bool m_RenderWireframe = false;

    //  Camera
    std::shared_ptr<Camera> m_Camera;

    // Shader, Texture, and Mesh
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_BaseColorTexture;

    // MVP matrices
    glm::mat4 m_ModelMatrix{0};
    glm::mat4 m_ProjectionMatrix{0};

    glm::vec3 m_CameraPosition{0};

    glm::vec3 m_LightPosition{100};

    // Window properties
    std::string m_WindowTitle;
    int m_Width{}, m_Height{};

    // Statistics
    int m_DrawCalls{};

    void RenderWorld(World &world);
};

} // namespace BloxxEngine
