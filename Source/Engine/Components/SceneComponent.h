#pragma once

#include "Camera/BasicCamera.h"
#include "Component.h"


namespace Component {
    /** @struct ISceneComponent
     *
     * @brief Base interface for components that have position related behaviour, but do not
     * have geometric representation
     */
    struct ISceneComponent : IComponent
    {
    };

    struct Transformation2D : ISceneComponent
    {
        Vec2 Position;
        float Rotation;
        Vec2 Scale;

        Transformation2D(Vec2 pos = Math::NullVec2, float rot = 0.f, Vec2 scale = Vec2{1.f, 1.f})
            : Position(std::move(pos)), Rotation(std::move(rot)), Scale(std::move(scale))
        {
        }

        [[nodiscard]] Mat4 const& GetModelMatrix() const { return Math::OrthoModelMatrix(Position, Rotation, Scale); }
    };

    struct Transformation : ISceneComponent
    {
        Vec3 Position{Math::NullVec3};
        Vec3 Rotation{Math::NullVec3};
        Vec3 Scale{Math::v3};

        Transformation(Vec3 position = Math::NullVec3, Vec3 rotation = Math::NullVec3, Vec3 scale = Math::v3)
            : Position(std::move(position)), Rotation(std::move(rotation)), Scale(std::move(scale))
        {
        }

        [[nodiscard]] Mat4 const& GetModelMatrix() const { return Math::ModelMatrix(Position, Rotation, Scale); }
    };

    struct Camera : ISceneComponent
    {
        Render::BasicCamera camera{};

        [[nodiscard]] const Mat4& GetProjection() const { return camera.GetProjection(); }
    };

    struct SpringArm : ISceneComponent
    {
        // float MaxLength;
        Entities::Entity* AttachedTo;
        float Length;
    };
} // namespace Component
