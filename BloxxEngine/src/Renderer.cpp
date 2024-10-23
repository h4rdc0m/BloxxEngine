/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/Renderer.h"

#include "BloxxEngine/EventDispatcher.h"
#include "BloxxEngine/Events/KeyboardEvents.h"
#include "BloxxEngine/Events/MouseEvents.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

namespace BloxxEngine
{

float Renderer::s_LastX = 400.0f;
float Renderer::s_LastY = 300.0f;
bool Renderer::s_FirstMouse = true;

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
    : m_Window(nullptr), m_Shader(nullptr), m_BaseColorTexture(nullptr), m_Mesh(nullptr), m_LastFrameTime(0.0f),
      m_WindowTitle("BloxxEngine"), m_Width(800), m_Height(600),
      m_Camera(std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), /* up vector */ glm::vec3(0.0f, 1.0f, 0.0f), /* yaw */ -90.0f, /* pitch */ 0.0f))
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

    if (!InitializeImGui())
        return false;

    // Load shaders
    m_Shader = std::make_unique<Shader>("Resources/shaders/block.vert.glsl", "Resources/shaders/block.frag.glsl");
    // Load texture
    m_BaseColorTexture = std::make_unique<Texture>("Resources/textures/Stone_basecolor.png", Texture::FilterMode::Nearest); // Provide the path to your texture image
    m_NormalTexture = std::make_unique<Texture>("Resources/textures/Stone_normal.png", Texture::FilterMode::Nearest); // Provide the path to your texture image
    m_RMAHTexture = std::make_unique<Texture>("Resources/textures/Stone_rmah.png", Texture::FilterMode::Nearest); // Provide the path to your texture image

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
    m_ProjectionMatrix =
        glm::perspective(glm::radians(45.0f), static_cast<float>(m_Width) / static_cast<float>(m_Height), 0.1f, 100.f);

    m_LastFrameTime = static_cast<float>(glfwGetTime());

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);

    glEnable(GL_DEPTH_TEST);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error during initialization: " << error << std::endl;
    }

    return true;
}

void Renderer::Run()
{
    MainLoop();
}

void Renderer::Shutdown()
{
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
}
void Renderer::OnKeyPressed(const KeyPressedEvent &event)
{
    switch (event.KeyCode)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
        break;
    case GLFW_KEY_W:
        m_Camera->Move(Camera::Movement::Forward, m_DeltaTime);
        break;
    case GLFW_KEY_S:
        m_Camera->Move(Camera::Movement::Backward, m_DeltaTime);
        break;
    case GLFW_KEY_A:
        m_Camera->Move(Camera::Movement::Left, m_DeltaTime);
        break;
    case GLFW_KEY_D:
        m_Camera->Move(Camera::Movement::Right, m_DeltaTime);
        break;
    case GLFW_KEY_SPACE:
        m_Camera->Move(Camera::Movement::Up, m_DeltaTime);
        break;
    case GLFW_KEY_LEFT_CONTROL:
        m_Camera->Move(Camera::Movement::Down, m_DeltaTime);
        break;
    default:
        break;
    }
}
void Renderer::OnMouseMoved(const MouseMovedEvent &event)
{
    m_Camera->Rotate(event.X, event.Y);
}
void Renderer::OnMouseScrolled(const MouseScrolledEvent &event)
{
    m_Camera->Zoom(event.OffsetY);
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

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetKeyCallback(m_Window, KeyCallback);
    glfwSetCursorPosCallback(m_Window, MouseMoveCallback);
    glfwSetScrollCallback(m_Window, MouseScrollCallback);

    // Capture the mouse
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

bool Renderer::InitializeImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // Enable DPI scaling
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // Enable DPI scaling

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    // Initialize ImGui GLFW backend
    if (!ImGui_ImplGlfw_InitForOpenGL(m_Window, true))
    {
        std::cerr << "Failed to initialize ImGui GLFW backend" << std::endl;
        return false;
    }

    // Initialize ImGui OpenGL backend
    if (!ImGui_ImplOpenGL3_Init("#version 330 core"))
    {
        std::cerr << "Failed to initialize ImGui OpenGL backend" << std::endl;
        return false;
    }

    return true;
}
void Renderer::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        bool repeat = action == GLFW_REPEAT;
        KeyPressedEvent event(key, repeat);
        renderer->OnEvent(event);
    }
}
void Renderer::MouseMoveCallback(GLFWwindow *window, double xPos, double yPos)
{
    auto *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));

    if (s_FirstMouse)
    {
        s_LastX = static_cast<float>(xPos);
        s_LastY = static_cast<float>(yPos);
        s_FirstMouse = false;
    }

    const auto xOffset = static_cast<float>(xPos - s_LastX);
    const auto yOffset = static_cast<float>(s_LastY - yPos); // Reversed since y-coordinates go from bottom to top

    s_LastX = static_cast<float>(xPos);
    s_LastY = static_cast<float>(yPos);

    renderer->m_Camera->Rotate(xOffset, yOffset);
}
void Renderer::MouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    auto *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
    renderer->OnEvent(event);
}

void Renderer::MainLoop()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        // Calculate delta time
        const auto currentFrameTime = static_cast<float>(glfwGetTime());
        m_DeltaTime = static_cast<float>(currentFrameTime - m_LastFrameTime);
        m_LastFrameTime = currentFrameTime;

        // Process input
        ProcessInput();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Update logic
        OnUpdate(m_DeltaTime);

        OnImGuiRender();
        ImGui::Render();
        ImGui::UpdatePlatformWindows();

        // Clear the screen
        glViewport(0, 0, m_Width, m_Height);
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render
        OnRender();

        // Render ImGui on top
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Renderer::ProcessInput() const
{
}

void Renderer::OnUpdate(float deltaTime)
{

    // Rotate the model
    float rotationSpeed = 50.0f; // degrees per second
    float angle = rotationSpeed * deltaTime;
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    // Rotate the light position around the Y-axis
    float lightRotationSpeed = 20.0f; // degrees per second
    float lightAngle = glm::radians(lightRotationSpeed * deltaTime);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), lightAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    // m_LightPosition = glm::vec3(rotationMatrix * glm::vec4(m_LightPosition, 1.0f));
}

void Renderer::OnRender()
{
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
    m_Shader->SetUniformInt("material.albedo", 0);
    m_RMAHTexture->Bind(1);
    m_Shader->SetUniformInt("material.rmah", 1);
    m_NormalTexture->Bind(2);
    m_Shader->SetUniformInt("material.normal", 2);

    // Set light properties
    m_Shader->SetUniformVec3("light.position", m_LightPosition);
    m_Shader->SetUniformVec3("light.color", glm::vec3(1.f));
    m_Shader->SetUniformVec3("light.ambient", glm::vec3(.06f));

    // Set the view position
    m_Shader->SetUniformVec3("viewPos", m_Camera->Position);

    // Draw the mesh
    m_Mesh->Draw();
    m_DrawCalls++;

    // Unbind everything
    m_Shader->Unbind();
    m_BaseColorTexture->Unbind();
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
}

void Renderer::OnImGuiRender()
{
    ImGui::Begin("Render statistics");
    ImGui::Text("FPS: %.2f", 1.0f / m_DeltaTime);
    ImGui::Text("Frame Time: %.2f ms", m_DeltaTime * 1000.0f);
    ImGui::Text("Draw calls: %d", m_DrawCalls);
    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", m_Camera->Position.x, m_Camera->Position.y, m_Camera->Position.z);
    ImGui::End();
}
void Renderer::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent &e) {
        OnKeyPressed(e);
        return true;
    });

    dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent &e) {
        OnMouseMoved(e);
        return true;
    });

    dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent &e) {
        OnMouseScrolled(e);
        return true;
    });
}

} // namespace BloxxEngine