/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/World/BlockRegistry.h"

#include <glad/gl.h>

#include "BloxxEngine/Renderer/RenderCommand.h"

namespace BloxxEngine {
void RenderCommand::SetClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void RenderCommand::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}
void RenderCommand::DisableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}

void RenderCommand::EnableWireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void RenderCommand::DisableWireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderCommand::SetViewport(const int x, const int y, const int width, const int height)
{
    glViewport(x, y, width, height);
}

} // BloxxEngine