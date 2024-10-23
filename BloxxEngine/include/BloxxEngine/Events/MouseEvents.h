/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "Event.h"

namespace BloxxEngine
{
class MouseMovedEvent final : public Event
{
    public:
    float X, Y;

    MouseMovedEvent(const float x, const float y) : X(x), Y(y) {}

    static EventType GetStaticType() { return EventType::MouseMoved; }
    [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
};

class MouseScrolledEvent final : public Event
{
  public:
    float OffsetX, OffsetY;

    MouseScrolledEvent(const float offsetX, const float offsetY) : OffsetX(offsetX), OffsetY(offsetY)
    {
    }
    static EventType GetStaticType() { return EventType::MouseScrolled; }
    [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
};
}