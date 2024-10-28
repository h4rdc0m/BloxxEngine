/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once

#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

#include <glad/gl.h>
#define GLFW_
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

namespace BloxxEngine
{
class Renderer;
class Camera;

struct WindowProps
{
    std::string Title;
    unsigned int Width, Height;

    explicit WindowProps(const std::string &title = "BloxxEngine", const unsigned int width = 1280,
                         const unsigned int height = 720)
        : Title(title), Width(width), Height(height)
    {
    }
};

class Engine
{
  public:
    Engine();
    virtual ~Engine();

    bool Init(const std::string &title, int width, int height);
    void Run();
    void Shutdown();

  protected:
    virtual void OnKeyPress(const KeyPressedEvent &event);
    virtual void OnKeyRelease(const KeyPressedEvent &event);
    virtual void OnMouseMove(const MouseMovedEvent &event);
    virtual void OnMouseScroll(const MouseScrolledEvent &event);

    virtual void OnUpdate(float deltaTime);
    virtual void OnRender();
    virtual void OnImGuiRender();

    void MainLoop();

    [[nodiscard]] GLFWwindow *GetWindow() const
    {
        return m_Window;
    }
    std::shared_ptr<Camera> &GetCamera()
    {
        return m_Camera;
    }

  private:
    bool InitGLFW();
    static bool InitGlad();
    [[nodiscard]] bool InitImGui() const;
    bool InitRenderer();
    void ProcessInput();

    void OnEvent(Event & event);

    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void FramebufferResizeCallback(GLFWwindow *window, int width, int height);



    GLFWwindow *m_Window;
    std::shared_ptr<Camera> m_Camera;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<WindowProps> m_Data;

    float m_LastFrameTime = 0.0f;
    float m_DeltaTime = 0.0f;

    bool m_CursorEnable = true;

    // Mouse state
    static float s_LastX, s_LastY;
    static bool s_FirstMouse;
};

} // namespace BloxxEngine
