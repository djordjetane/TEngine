#pragma once
#include "Keys.h"

namespace Input {

    struct IEvent
    {
        bool IsActive = false;
    };

    struct KeyEvent : IEvent
    {
        EKeyAction Action{};
        EKeyEventState ActionTriggerState{EKeyEventState::PRESSED};
        KeyEvent(EKeyAction _action, EKeyEventState _activeState = EKeyEventState::PRESSED)
            : Action(std::move(_action)), ActionTriggerState(_activeState)
        {
        }
    };
} // namespace Input