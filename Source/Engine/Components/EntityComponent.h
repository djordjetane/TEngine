#pragma once

#include <Input/InputEvent.h>
#include "Component.h"

using KeyEvent = Input::KeyEvent;

namespace Component {
    struct IEntityComponent : IComponent
    {
    };

    struct Input : IEntityComponent
    {
        std::vector<KeyEvent> Events;
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

    struct HealthComponent : IEntityComponent
    {
        float MaxHealth;
        float CurrentHealth;
    };
} // namespace Component
