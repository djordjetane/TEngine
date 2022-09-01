#pragma once
#include "BasicCamera.h"
namespace Window
{
    class Gui;
}
namespace Render {

    class Camera : public BasicCamera
    {
    public:
        Camera(float fov = 45.f);
        Camera(const Mat4& projection);

        BasicCamera GetCamera();

        void LookUp();
        void LookDown();
        void LookRight();
        void LookLeft();

        [[nodiscard]] Mat4 LookAt() const;
        void SetCameraSpeed(float dt);

        [[nodiscard]] const Vec3& GetCameraPos() const;
        void SetCameraPos(Vec3 pos);

        [[nodiscard]] float GetCameraZoom() const;

        [[nodiscard]] const Vec3& GetCameraFront() const;
        [[nodiscard]] Quat Camera::GetOrientation() const;
        [[nodiscard]] Vec3 Camera::GetRotation() const;


        void UpdateMovement(float xoffset, float yoffset);
        void UpdateZoom(float yoffset);

    private:
        Mat4 m_View{Math::I4};

        Vec3 cameraPos{0.f, 0.5f, 3.f};
        Vec3 cameraFront{0.f, 0.f, -1.f};
        Vec3 cameraUp{0.f, 1.f, 0.f};
        float cameraSpeed = 0.5f;

        float m_Yaw              = -90.f;
        float m_Pitch            = 0.f;
        float m_MovementSpeed    = 4.5f;
        float m_FOVy              = 45.f;

        /* Bounds */
        const float constrainPitchBound = 89.f;
        const float zoomLowerBound      = 1.f;
        const float zoomUpperBound      = 45.f;

        void m_UpdateCameraVectors();

        /* Keeping angles in range [lower, upper], to prevent flipping */
        inline void m_BoundAngleInRange(float& angle, float lower, float upper) const
        {
            angle = std::max(lower, angle);
            angle = std::min(angle, upper);
        }

#ifdef _DEBUG
        friend Window::Gui;
#endif
    };

} // namespace Render