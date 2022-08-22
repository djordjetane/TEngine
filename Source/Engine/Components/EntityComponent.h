#pragma once

#include <Input/InputEvent.h>
#include "Component.h"

using KeyEvent = Input::KeyEvent;

namespace Component {
    struct IEntityComponent : IComponent
    {
    };

    struct KeyInput : IEntityComponent
    {
        std::vector<KeyEvent> Events;
    };

    struct MInput : IEntityComponent
    {
        // double Xpos, Ypos;
        Vec2 PositionOffset{0.f};
        double ScrollOffset = 0.0;
        float  Sensitivity  = 0.1f;
    };

    struct Player : IEntityComponent
    {
    };

    struct Movement : IEntityComponent
    {
        Vec3 Speed{0.f};
        Vec3 Rotation{0.f, 0.f, 0.f};
        float friction = 0.f;
    };

    struct Health : IEntityComponent
    {
        float MaxHealth;
        float CurrentHealth;
    };
} // namespace Component
