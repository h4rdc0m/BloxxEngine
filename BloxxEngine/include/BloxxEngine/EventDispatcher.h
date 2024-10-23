/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include "Events/Event.h"

namespace BloxxEngine
{
class EventDispatcher
{
    public:
    explicit EventDispatcher(Event& event) : m_Event(event) {}

    template<typename T, typename F>
    bool Dispatch(const F& func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

    private:
    Event& m_Event;
};
}