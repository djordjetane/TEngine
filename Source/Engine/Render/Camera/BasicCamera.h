#pragma once
#include <Core/Math.h>

namespace Render {
    class BasicCamera
    {
    public:
        BasicCamera(Mat4 projMatrix = Math::I4) : m_Projection(std::move(projMatrix)) {}

        [[nodiscard]] const glm::mat4& GetProjection() const { return m_Projection; }

    private:
        Mat4 m_Projection;
    };
} // namespace Render
