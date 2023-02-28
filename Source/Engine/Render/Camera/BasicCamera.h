#pragma once
#include <Core/Math.h>

namespace Render {
    class BasicCamera
    {
    public:
        BasicCamera(float fovy = 45.f, float aspect = WindowData::GetAspect(), float zNear = 0.1f, float zFar = 1000.f)
    	: m_Projection(std::move(Math::PerspectiveMatrix(fovy, aspect, zNear, zFar))) {}

        BasicCamera(Mat4 projMatrix) : m_Projection(std::move(projMatrix)) {}
        [[nodiscard]] const glm::mat4& GetProjection() const { return m_Projection; }

    private:
        Mat4 m_Projection;
    };
} // namespace Render
