/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include <memory>

#include "BloxxEngine/Engine.h"

#include "BloxxEngine/Core/Log.h"
#include "BloxxEngine/EventDispatcher.h"
#include "BloxxEngine/Renderer/Renderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace BloxxEngine
{

float Engine::s_LastX = 400.0f;
float Engine::s_LastY = 300.0f;
bool Engine::s_FirstMouse = true;

// Error callback for GLFW
static void GLFWErrorCallback(int error, const char *description)
{
    BE_CORE_ERROR("GLFW Error ({}): {}", error, description);
}

Engine::Engine() : m_Window(nullptr), m_Camera(nullptr), m_Renderer(nullptr)
{
    m_Data = std::make_unique<WindowProps>("BloxxEngine", 1280, 760);

}
Engine::~Engine()
{
    Shutdown();
}

bool Engine::Init(const std::string &title, int width, int height)
{
    Log::Init();

    BE_CORE_INFO("Initializing engine...");
    m_Data = std::make_unique<WindowProps>(title, width, height);

    if (!InitGLFW())
        return false;

    if (!InitGlad())
        return false;

    if (!InitImGui())
        return false;

    if (!InitRenderer())
        return false;

    m_LastFrameTime = static_cast<float>(glfwGetTime());

    return true;
}
void Engine::Run()
{
    MainLoop();
}

void Engine::Shutdown()
{
    m_Renderer->Shutdown();

    if (m_Window != nullptr)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    glfwTerminate();
}

void Engine::OnUpdate(const float deltaTime)
{
    m_Renderer->Update(deltaTime);
}

void Engine::OnRender()
{
    m_Renderer->Render();
}

void Engine::OnImGuiRender() {

}

void Engine::MainLoop()
{
    // Timing variables
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();

    while (!glfwWindowShouldClose(m_Window))
    {
        // Calculate deltaTime
        auto currentTime = clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        m_DeltaTime = elapsed.count();
        lastTime = currentTime;

        // Process input
        ProcessInput();

        // Update logic
        OnUpdate(m_DeltaTime);

        OnImGuiRender();

        // Render
        OnRender();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

bool Engine::InitGLFW()
{
    BE_CORE_INFO("Initializing GLFW...");
    glfwSetErrorCallback(GLFWErrorCallback);

    if (!glfwInit())
    {
        BE_CORE_FATAL("Failed to initialize GLFW");
        return false;
    }

    // Set window hints for OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(static_cast<int>(m_Data->Width), static_cast<int>(m_Data->Height),
                                m_Data->Title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        BE_CORE_FATAL("Failed to create GLFW window");
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
    glfwSetFramebufferSizeCallback(m_Window, FramebufferResizeCallback);

    // Capture the mouse
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    BE_CORE_INFO("GLFW initialized.");
    return true;
}
bool Engine::InitGlad()
{
    BE_CORE_INFO("Initializing GLAD...");
    if (!gladLoadGL(glfwGetProcAddress))
    {
        BE_CORE_FATAL("Failed to initialize GLAD");
        return false;
    }
    BE_CORE_INFO("GLAD initialized.");
    return true;
}
bool Engine::InitImGui() const
{
    BE_CORE_INFO("Initializing ImGui...");
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
        BE_CORE_FATAL("Failed to initialize ImGui GLFW backend");
        return false;
    }

    // Initialize ImGui OpenGL backend
    if (!ImGui_ImplOpenGL3_Init("#version 330 core"))
    {
        BE_CORE_FATAL("Failed to initialize ImGui OpenGL backend");
        return false;
    }

    BE_CORE_INFO("ImGui initialized.");
    return true;
}
bool Engine::InitRenderer()
{
    BE_CORE_INFO("Initializing Renderer");
    m_Camera = std::make_shared<Camera>(glm::vec3(-10.f, 15.f, 10.f), glm::vec3(0.f, 1.f, 0.f), 0.f, 0.f);
    m_Renderer = std::make_unique<Renderer>(m_Camera, m_Data->Width, m_Data->Height);
    return m_Renderer->Init();
}
void Engine::ProcessInput()
{
    // Example usage with GLFW key polling
    bool forward = glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS;
    bool backward = glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS;
    bool left = glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS;
    bool right = glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS;
    bool up = glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool down = glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    m_Camera->Move(forward, backward, left, right, up, down,m_DeltaTime);
}

void Engine::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        const bool repeat = action == GLFW_REPEAT;
        KeyPressedEvent event(key, repeat);
        engine->OnEvent(event);
    }
}
void Engine::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
}

void Engine::MouseMoveCallback(GLFWwindow *window, double xPos, double yPos)
{
    auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));

    if (!engine->m_CursorEnable)
    {
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

        engine->m_Camera->Rotate(xOffset, yOffset);
    }
}
void Engine::MouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
    MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
    engine->OnEvent(event);
}
void Engine::FramebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);

    // Update renderer's width and height
    engine->m_Data->Width = width;
    engine->m_Data->Height = height;

    engine->m_Renderer->SetProjectionMatrix(glm::perspective(
        glm::radians(45.0f), static_cast<float>(engine->m_Data->Width) / static_cast<float>(engine->m_Data->Height),
        0.1f, 100.f));
}

// EVENTS
void Engine::OnKeyPress(const KeyPressedEvent &event)
{
    switch (event.KeyCode)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
        break;
    case GLFW_KEY_F1:
        m_Renderer->SetRenderWireframe(!m_Renderer->GetRenderWireframe());
        break;
    case GLFW_KEY_F2:
        m_CursorEnable = !m_CursorEnable;
        if (m_CursorEnable)
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;

    default:
        break;
    }
}

void Engine::OnKeyRelease(const KeyPressedEvent &event)
{
    switch (event.KeyCode)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
        break;
    case GLFW_KEY_F1:
        m_Renderer->SetRenderWireframe(!m_Renderer->GetRenderWireframe());
        break;
    case GLFW_KEY_F2:
        m_CursorEnable = !m_CursorEnable;
        if (m_CursorEnable)
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;

    default:
        break;
    }
}

void Engine::OnMouseMove(const MouseMovedEvent &event)
{
    m_Camera->Rotate(event.X, event.Y);
}

auto Engine::OnMouseScroll(const MouseScrolledEvent &event) -> void
{
    m_Camera->Zoom(event.OffsetY);
}

void Engine::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent &e) {
        OnKeyPress(e);
        return true;
    });

    dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent &e) {
        OnMouseMove(e);
        return true;
    });

    dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent &e) {
        OnMouseScroll(e);
        return true;
    });
}
} // namespace BloxxEngine