/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once

namespace BloxxEngine
{
enum class EventType
{
    None = 0,
    KeyPressed, KeyReleased,
    MouseMoved, MouseScrolled,
    WindowResized,
    // Add more as needed
};

class Event
{
public:
    [[nodiscard]] virtual EventType GetEventType() const = 0;
    virtual ~Event() = default;
};
}