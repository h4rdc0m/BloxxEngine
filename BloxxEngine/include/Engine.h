/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */


#pragma once

#include <cstdint>
#include <string>

namespace BE {

class Engine {
public:
 Engine(std::string title, uint16_t width, uint16_t height);
 ~Engine() = default;

 void run();

 private:
 std::string m_Title;
 uint16_t m_Width, m_Height;

 void Init();
 void Update();
 void Shutdown();
};

} // BE
