/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/Renderer.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace BloxxEngine
{

// Error callback for GLFW
static void GLFWErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam)
{
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = 0x" << std::hex
              << type << ", severity = 0x" << severity << ", message = " << message << std::endl;
}

Renderer::Renderer()
    : m_Window(nullptr), m_Shader(nullptr), m_Texture(nullptr), m_Mesh(nullptr), m_LastFrameTime(0.0f),
      m_WindowTitle("BloxxEngine"), m_Width(800), m_Height(600)
{
}

Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize(const std::string &windowTitle, int width, int height)
{
    m_WindowTitle = windowTitle;
    m_Width = width;
    m_Height = height;

    if (!InitializeGLFW())
        return false;
    if (!InitializeGlad())
        return false;

    // Load shaders
    m_Shader = std::make_unique<Shader>("Resources/shaders/block.vert.glsl", "Resources/shaders/block.frag.glsl");
    // Load texture
    m_Texture = std::make_unique<Texture>("Resources/textures/T_Stone.png",
                                          Texture::FilterMode::Nearest); // Provide the path to your texture image

    // clang-format off
    // Cube vertices with positions, normals, and texture coordinates
    std::vector<Vertex> vertices = {
        // Positions          // Normals           // Texture Coords
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // Top-left

        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // Top-left
        {{-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // Top-right

        // Left face
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // Top-left

        // Right face
        {{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f, -0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // Top-left
        {{ 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // Top-right

        // Top face
        {{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // Top-left
        {{-0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // Top-right

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // Top-left
    };

    std::vector<GLuint> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20,
    };

    // clang-format on
    m_Mesh = std::make_unique<Mesh>(vertices, indices);

    // Set up matrices
    m_ModelMatrix = glm::mat4(1.0f);
    m_ViewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 3.f), // Camera position
                               glm::vec3(0.f, 0.f, 0.f), // Target position
                               glm::vec3(0.f, 1.f, 0.f)  // Up vector
    );
    m_ProjectionMatrix =
        glm::perspective(glm::radians(45.0f), static_cast<float>(m_Width) / static_cast<float>(m_Height), 0.1f, 100.f);

    m_LastFrameTime = static_cast<float>(glfwGetTime());

    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(MessageCallback, nullptr);

    glEnable(GL_DEPTH_TEST);

    return true;
}

void Renderer::Run()
{
    MainLoop();
}

void Renderer::Shutdown()
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
}

bool Renderer::InitializeGLFW()
{
    glfwSetErrorCallback(GLFWErrorCallback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Set window hints for OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowTitle.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);

    // Enable V-Sync
    glfwSwapInterval(1);

    return true;
}

bool Renderer::InitializeGlad()
{
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

void Renderer::MainLoop()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        // Calculate delta time
        const auto currentFrameTime = static_cast<float>(glfwGetTime());
        auto deltaTime = static_cast<float>(currentFrameTime - m_LastFrameTime);
        m_LastFrameTime = currentFrameTime;

        // Process input
        ProcessInput();

        // Update logic
        OnUpdate(deltaTime);

        // Clear the screen
        glViewport(0, 0, m_Width, m_Height);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render
        OnRender();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Renderer::ProcessInput() const
{
    // Close window on ESC key press
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);
}

void Renderer::OnUpdate(float deltaTime)
{
    // Update the model matrix
    const float angle = static_cast<float>(glfwGetTime()) * glm::radians(50.0f);
    m_ModelMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.5f, 1.0f, 0.0f));

    const auto lightPos = glm::vec3(1.2f * sin(glfwGetTime()), 1.0f, 2.0f * cos(glfwGetTime()));
    m_Shader->SetUniformVec3("light.position", lightPos);
}

void Renderer::OnRender()
{
    // Set matrices
    m_Shader->Bind();

    m_Shader->SetUniformMat4("model", m_ModelMatrix);
    m_Shader->SetUniformMat4("view", m_ViewMatrix);
    m_Shader->SetUniformMat4("projection", m_ProjectionMatrix);

    // Set material properties
    m_Texture->Bind(0);
    m_Shader->SetUniformInt("material.diffuse", 0);
    m_Shader->SetUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    m_Shader->SetUniformFloat("material.shininess", 32.0f);

    // Set light properties
    m_Shader->SetUniformVec3("light.position", glm::vec3(1.0f, 1.0f, 2.0f));
    m_Shader->SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    m_Shader->SetUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    m_Shader->SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // Set the view position
    constexpr auto cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    m_Shader->SetUniformVec3("viewPos", cameraPos);

    // Draw the mesh
    m_Mesh->Draw();

    // Unbind everything
    m_Texture->Unbind();
    m_Shader->Unbind();
}

void Renderer::OnImGuiRender()
{

}

} // namespace BloxxEngine