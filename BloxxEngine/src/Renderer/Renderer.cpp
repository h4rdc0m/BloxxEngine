/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/Renderer/Renderer.h"

#include "BloxxEngine/Renderer/RenderCommand.h"
#include "BloxxEngine/World/World.h"

#include <chrono>
#include <glad/gl.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "BloxxEngine/Core/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>


namespace BloxxEngine
{

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam)
{
    BE_CORE_ERROR("GL CALLBACK: {} type = 0x{}, severity = 0x{}, message = {}", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

Renderer::Renderer(const std::shared_ptr<Camera> &camera, const int width, const int height)
    : m_Camera(camera), m_Shader(nullptr), m_BaseColorTexture(nullptr), m_Width(width), m_Height(height)
{
}

Renderer::~Renderer() = default;

bool Renderer::Init()
{
    // Load shaders
    m_Shader = std::make_unique<Shader>("shaders/block.vert.glsl", "shaders/minecraft.frag.glsl");
    // Load texture
    m_BaseColorTexture =
        std::make_unique<Texture>("textures/stone.png",
                                  Texture::FilterMode::Nearest); // Provide the path to your texture image

    // Set up matrices
    m_ModelMatrix = glm::mat4(1.0f);
    m_ProjectionMatrix =
        glm::perspective(glm::radians(45.0f), static_cast<float>(m_Width) / static_cast<float>(m_Height), 0.1f, 100.f);

    RenderCommand::EnableDepthTest();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    if (const GLenum error = glGetError(); error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error during initialization: " << error << std::endl;
    }

    return true;
}

void Renderer::Shutdown()
{
    // Cleanup ImGui
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

void Renderer::Render(World &world)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Finally render ImGui
    ImGuiRender();
    ImGui::Render();
    ImGui::UpdatePlatformWindows();

    RenderCommand::SetViewport(0, 0, m_Width, m_Height);
    RenderCommand::SetClearColor(glm::vec4(0.529f, 0.808f, 0.922f, 1.0f));
    RenderCommand::Clear();

    if (m_RenderWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_DrawCalls = 0;
    // Set matrices
    m_Shader->Bind();

    m_Shader->SetUniformMat4("model", m_ModelMatrix);
    m_Shader->SetUniformMat4("view", m_Camera->GetViewMatrix());
    m_ProjectionMatrix = glm::perspective(glm::radians(m_Camera->ZoomFactor),
                                          static_cast<float>(m_Width) / static_cast<float>(m_Height), 0.1f, 100.0f);
    m_Shader->SetUniformMat4("projection", m_ProjectionMatrix);

    // Set material properties
    m_BaseColorTexture->Bind(0);
    m_Shader->SetUniformInt("albedo", 0);

    // Set light properties
    m_Shader->SetUniformVec3("lightPos", m_LightPosition);
    m_Shader->SetUniformVec3("lightColor", glm::vec3(1.f));
    m_Shader->SetUniformVec3("ambientLight", glm::vec3(.6f));

    // Set the view position
    // m_Shader->SetUniformVec3("viewPos", m_Camera->Position);
    RenderWorld(world);

    // Unbind everything
    m_Shader->Unbind();
    m_BaseColorTexture->Unbind();
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    // Render ImGui on top
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Renderer::Update(const float deltaTime)
{
    m_DeltaTime = deltaTime;
}

void Renderer::ImGuiRender()
{
    ImGui::Begin("Render statistics");
    ImGui::Text("FPS: %.2f", 1.0f / m_DeltaTime);
    ImGui::Text("Frame Time: %.2f ms", m_DeltaTime * 1000.0f);
    ImGui::Text("Draw calls: %d", m_DrawCalls);
    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", m_Camera->Position.x, m_Camera->Position.y,
                m_Camera->Position.z);
    ImGui::End();
}
void Renderer::RenderWorld(World &world)
{
    world.Draw();
}

} // namespace BloxxEngine