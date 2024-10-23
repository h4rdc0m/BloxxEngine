/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "Event.h"

namespace BloxxEngine
{
class KeyEvent : public Event
{
  public:
    int KeyCode;

    explicit KeyEvent(const int KeyCode) : KeyCode(KeyCode)
    {
    }

    ~KeyEvent() override = default;
};

class KeyPressedEvent final : public KeyEvent
{
  public:
    bool Repeat;

    KeyPressedEvent(const int keyCode, const bool repeat) : KeyEvent(keyCode), Repeat(repeat)
    {
    }
    static EventType GetStaticType() { return EventType::KeyPressed; }
    [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
};

class KeyReleasedEvent final : public KeyEvent
{
  public:
    KeyReleasedEvent(const int keyCode, const bool repeat) : KeyEvent(keyCode)
    {
    }
    static EventType GetStaticType() { return EventType::KeyReleased; }
    [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
};
} // namespace BloxxEngine