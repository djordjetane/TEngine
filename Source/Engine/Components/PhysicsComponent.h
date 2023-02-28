#pragma once

#include "Component.h"

namespace Component {
    struct IPhysicsComponent : IComponent
    {
    };

    struct TerrainColider : IPhysicsComponent
    {
        std::vector<std::vector<float>> HeightMap;
    };

    struct SphereColider : IPhysicsComponent
    {
        float Radius;
        Vec3 Center; // From entity's local space
    };
} // namespace Component
