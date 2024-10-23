/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include "Camera.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
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

    virtual void OnKeyPressed(const KeyPressedEvent &event);
    virtual void OnMouseMoved(const MouseMovedEvent &event);
    virtual void OnMouseScrolled(const MouseScrolledEvent & event);

  protected:
    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();
    virtual void OnImGuiRender(); // For future ImGUI integration
    void OnEvent(Event & event);

  private:
    bool InitializeGLFW();
    static bool InitializeGlad();
    bool InitializeImGui();


    // GLFW callbacks
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
    static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

    void MainLoop();

    void ProcessInput() const;

    // GLFW window
    GLFWwindow *m_Window;

    // Mouse state
    static float s_LastX, s_LastY;
    static bool s_FirstMouse;

    //  Camera
    std::unique_ptr<Camera> m_Camera;

    // Shader, Texture, and Mesh
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_BaseColorTexture;
    std::unique_ptr<Texture> m_NormalTexture;
    std::unique_ptr<Texture> m_RMAHTexture;
    std::unique_ptr<Mesh> m_Mesh;

    // Timing
    float m_LastFrameTime;
    float m_DeltaTime{};

    // MVP matrices
    glm::mat4 m_ModelMatrix{0};
    glm::mat4 m_ProjectionMatrix{0};

    glm::vec3 m_CameraPosition{0};

    glm::vec3 m_LightPosition{1};

    // Window properties
    std::string m_WindowTitle;
    int m_Width, m_Height;

    // Statistics
    int m_DrawCalls{};
};

} // namespace BloxxEngine
