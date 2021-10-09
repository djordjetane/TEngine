#include <precomp.h>

#include "Camera.h"

namespace Render {
    Camera::Camera(float fov) // option for ortho ??
        : BasicCamera(Math::PerspectiveMatrix(fov, WindowData::GetAspect(), 0.1f, 1000.f))
    {
    }

    Camera::Camera(const Mat4& projection) : BasicCamera(projection) {}

    BasicCamera Camera::GetCamera() { return BasicCamera{GetProjection()}; }

    void Camera::LookUp() { cameraPos += cameraSpeed * cameraFront; }

    void Camera::LookDown() { cameraPos -= cameraSpeed * cameraFront; }

    void Camera::LookRight() { cameraPos += cameraSpeed * Math::Normalize(Math::Cross(cameraFront, cameraUp)); }

    void Camera::LookLeft() { cameraPos -= cameraSpeed * Math::Normalize(Math::Cross(cameraFront, cameraUp)); }

    glm::mat4 Camera::LookAt() const { return Math::ViewMatirx(cameraPos, cameraPos + cameraFront, cameraUp); }

    void Camera::SetCameraSpeed(float dt) { cameraSpeed = m_MovementSpeed * dt; }

    const glm::vec3& Camera::GetCameraPos() const { return cameraPos; }

    void Camera::SetCameraPos(Vec3 pos) { cameraPos = std::move(pos); }

    float Camera::GetCameraZoom() const { return m_FOV; }

    const glm::vec3& Camera::GetCameraFront() const { return cameraFront; }

    glm::quat Camera::GetOrientation() const { return Quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f)); }

    Vec3 Camera::GetRotation() const
    {
        auto q{GetOrientation()};
        return Vec3{q.x, q.y, q.z};
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        m_BoundAngleInRange(m_Pitch, -constrainPitchBound, constrainPitchBound);

        // update Front, Right and Up Vectors using the updated Euler angles
        m_UpdateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yoffset)
    {
        m_FOV -= yoffset;
        m_BoundAngleInRange(m_FOV, zoomLowerBound, zoomUpperBound);
    }

    void Camera::m_UpdateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x     = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y     = sin(glm::radians(m_Pitch));
        front.z     = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        cameraFront = Math::Normalize(front);
        // also re-calculate the Right and Up vector
        LookRight();
        LookUp();
    }

} // namespace Render