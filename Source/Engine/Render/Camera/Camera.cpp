#include <precomp.h>

#include "Camera.h"

namespace Render {
    Camera::Camera(float fovyRad) // option for ortho ??
        :
        BasicCamera(Math::PerspectiveMatrix(fovyRad, WindowData::GetAspect(), 0.1f, 1000.f))
    {
    }

    Camera::Camera(const Mat4& projection) : BasicCamera(projection) {}

    BasicCamera Camera::GetCamera() { return BasicCamera{GetProjection()}; }

    void Camera::LookUp() { cameraPos += cameraSpeed * cameraFront; }

    void Camera::LookDown() { cameraPos -= cameraSpeed * cameraFront; }

    void Camera::LookRight() { cameraPos += cameraSpeed * Math::Normalize(Math::Cross(cameraFront, cameraUp)); }

    void Camera::LookLeft() { cameraPos -= cameraSpeed * Math::Normalize(Math::Cross(cameraFront, cameraUp)); }

    Mat4 Camera::LookAt() const { return Math::ViewMatirx(cameraPos, cameraPos + cameraFront, cameraUp); }

    void Camera::SetCameraSpeed(float dt) { cameraSpeed = m_MovementSpeed * dt; }

    const glm::vec3& Camera::GetCameraPos() const { return cameraPos; }

    void Camera::SetCameraPos(Vec3 pos) { cameraPos = std::move(pos); }

    float Camera::GetCameraZoom() const { return m_FOVy; }

    const glm::vec3& Camera::GetCameraFront() const { return cameraFront; }

    Quat Camera::GetOrientation() const { return Quat{glm::vec3(-m_Pitch, -m_Yaw, 0.0f)}; }

    Vec3 Camera::GetRotation() const
    {
        return MakeVec3<float>(&GetOrientation()[0]);
    }

    void Camera::UpdateMovement(float xoffset, float yoffset)
    {
        //xoffset *= m_MouseSensitivity;
        //yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        m_BoundAngleInRange(m_Pitch, -constrainPitchBound, constrainPitchBound);

        // update Front, Right and Up Vectors using the updated Euler angles
        m_UpdateCameraVectors();
    }

    void Camera::UpdateZoom(float yoffset)
    {
        m_FOVy -= yoffset;
        m_BoundAngleInRange(m_FOVy, zoomLowerBound, zoomUpperBound);
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