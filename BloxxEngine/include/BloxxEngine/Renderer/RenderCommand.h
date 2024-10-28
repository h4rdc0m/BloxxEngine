/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include <glm/vec4.hpp>

namespace BloxxEngine {

class RenderCommand {
public:
    static void SetClearColor(const glm::vec4& color);
    static void Clear();

    static void EnableDepthTest();
    static void DisableDepthTest();

    static void EnableWireframeMode();
    static void DisableWireframeMode();

    static void SetViewport(int x, int y, int width, int height);
};

} // BloxxEngine
