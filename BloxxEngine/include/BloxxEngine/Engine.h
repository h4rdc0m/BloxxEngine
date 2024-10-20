/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once

#include "GLFW/glfw3.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
#include <string>

namespace BE
{

class Engine
{
  public:
    explicit Engine(const std::string &title = "BloxxEngine", const std::string &appVersion = "v0.0.1",
                    const std::string &packageIdentifier = "nl.combat.bloxx-engine", int width = 800, int height = 600);
    ~Engine();

    bool Initialize();
    void Update();
    void Render();
    void Shutdown();
    void Run();

  private:
    GLFWwindow *m_Window = nullptr;
    std::string m_Title;
    std::string m_Version = "v0.0.1";
    std::string m_AppVersion = "v0.0.1";
    std::string m_PackageIdentifier;
    uint16_t m_Width, m_Height;
    std::string m_packageIdentifier;
    std::queue<std::function<void()>> m_RenderCommands;
};

extern Engine *CreateEngine();
} // namespace BE
