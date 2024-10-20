/*
 * Copyright (c) 2024 Combat Jongerenmarketing en -communicatie B.V.
 * All rights reserved.
 */

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <utility>

#include "BloxxEngine/Engine.h"

#include <iostream>

#include <ostream>
#include <stdexcept>

namespace BE
{

static void GLFWerrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error: " << description << std::endl;
}

Engine::Engine(const std::string& title, const std::string& appVersion, const std::string& packageIdentifier, const int width,
               const int height)
    : m_Title(title), m_AppVersion(appVersion), m_PackageIdentifier(packageIdentifier),
      m_Width(width), m_Height(height)
{
    glfwSetErrorCallback(GLFWerrorCallback);

    if (!Initialize())
    {
        throw std::runtime_error("Failed to initialize!");
    }
}

Engine::~Engine()
{
    Shutdown();
}

bool Engine::Initialize()
{
    if (!glfwInit())
        return false;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        return false;
    }

    gladLoadGL();

    return true;
}
void Engine::Run()
{
    glfwMakeContextCurrent(m_Window);

    while (!glfwWindowShouldClose(m_Window))
    {
        Update();
        Render();

        glfwSwapBuffers(m_Window);

        glfwPollEvents();
    }
}
void Engine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::Update()
{
    // update different systems/game logic here
    // add possibility of overriding from the application
    // maybe make the engine an executable, and load the game dll.
}

void Engine::Shutdown()
{
    glfwTerminate();
}

} // namespace BE