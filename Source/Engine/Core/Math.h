#pragma once
#include <types.h>


namespace Math {
    /* Types */
    using Array  = FVector;
    using Matrix = TVector<FVector>;
    struct Point2D
    {
        float x, y;
    };
    struct Point3D
    {
        float x, y, z;
    };


    /* Constants */
    constexpr double Pi = 3.14159265359;
    constexpr Mat4 I4{1.f};
    constexpr Vec3 e3{1.f};
    constexpr Vec3 v3{1.f, 1.f, 1.f};
    constexpr Vec3 NullVec2{0.f};
    constexpr Vec3 NullVec3{0.f};

    inline float Sqrt(float val) { return sqrtf(val); }

    inline Vec3 Normalize(const Vec3& v) { return glm::normalize(v); }
    inline Vec3 Cross(const Vec3& v1, const Vec3& v2) { return glm::cross(v1, v2); }

    inline Vec3 NormalOnDirection(const glm::vec3& direction)
    {
        return glm::normalize(glm::cross(direction, {0.f, 1.f, 0.f}));
    }

    inline Vec3 Degrees(const Vec3& radians) { return glm::degrees(radians); }

    inline Vec3 Radians(const Vec3& degrees) { return glm::radians(degrees); }

    template <int L, typename T>
    inline T Length(const Vec<L, T>& v)
    {
        return glm::length(v);
    }

    template <typename T>
    inline double Gaussian(T x, T y, double sigma)
    {
        return exp(-(x * x + y * y) / (2.f * sigma * sigma)) / (2.f * Pi * sigma * sigma);
    }

    inline Mat4 Translate(Mat4 const& m, const Vec3& v) { return glm::translate(m, v); }
    inline Mat4 Translation(const Vec3& v) { return glm::translate(I4, v); }
    inline Mat4 Rotate(Mat4 const& m, float angle, const Vec3& v) { return glm::rotate(m, angle, v); }
    inline Mat4 Rotation(float angle, const Vec3& v) { return glm::rotate(I4, angle, v); }
    inline Mat4 Scale(Mat4 const& m, const Vec3& scale) { return glm::scale(m, scale); }
    inline Mat4 Scale(Mat4 const& m, float scale) { return glm::scale(m, scale * e3); }

    /* Eulerian angles */
    /**
     * @brief Rotation matrix from eulerian angles in radians
     * @param x Roll around x-axis
     * @param y Pitch around y-axis
     * @param z Yaw around z-axis
     *
     * @return 4x4 rotation matrix
     */
    Mat4 EulerAnglesRotate(float x, float y, float z);


    /**
     * @see EulerAnglesRotate(float x, float y, float z)
     */
    Mat4 EulerAnglesRotate(const Vec3& degrees);

    // Translate * Rotate * Scale
    Mat4 ModelMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale);

    Mat4 OrthoModelMatrix(const Vec2& position, float rotation, const Vec2& scale);

    /**
     * @brief Perspective type of projection matrix
     * @param fov Field-of-view in degrees
     * @param aspect Aspect ratio
     * @param nearZ Near clipping plane
     * @param farZ Far clipping plane
     *
     * @return 4x4 rotation matrix
     */
    inline Mat4 PerspectiveMatrix(float fov, float aspect, float nearZ, float farZ)
    {
        return glm::perspective(glm::radians(fov), aspect, nearZ, farZ);
    }


    //inline Vec3 FrontCamera(const Vec3& rot)
    //{
    //    return Math::Normalize(Vec3{cos(glm::radians(rot.z)) * cos(glm::radians(rot.y)), 
	//								sin(glm::radians(rot.y)),
    //                                sin(glm::radians(rot.z)) * cos(glm::radians(rot.y))});
    //


    /**
     * @brief Orientation
     * @param pitch Rotation angle around x-axis in degrees
     * @param yaw Rotation angle around y-axis in degrees
     *
     */
    inline Quat GetOrientation(float pitch, float yaw)
    {
        return Quat{Vec3{-glm::radians(pitch), -glm::radians(yaw), 0.f}};
    }

    /**
     * @brief Calculating rotation vector using quaternion
     * @param pitch Rotation angle around x-axis in degrees
     * @param yaw Rotation angle around y-axis in degrees
     *
     * @returns rotation vector
     */
    inline Vec3 GetRotation(float pitch, float yaw)
    {
        return Normalize(MakeVec3<float>(&GetOrientation(pitch,yaw)[0]));
    }
    /*
    inline Vec3 FrontCamera(const Vec3& rot)
    {
        return Math::Normalize(Vec3{cos(glm::radians(rot.y)) * cos(glm::radians(rot.x)), 
    								sin(glm::radians(rot.x)),
                                    sin(glm::radians(rot.y)) * cos(glm::radians(rot.x))});
    }
    */


    inline Vec3 FrontCamera(Vec3&& rot)
    {
        return Math::Normalize(Vec3{cos(rot.y) * cos(rot.x), 
    								sin(rot.x),
                                    sin(rot.y) * cos(rot.x)});
    }

    inline Vec3 FrontCamera(const Vec3& rot)
    {
        return Math::Normalize(Vec3{cos(rot.y) * cos(rot.x), 
    								sin(rot.x),
                                    sin(rot.y) * cos(rot.x)});
    }

    inline Mat4 ViewMatrix(const Vec3& pos, const Vec3& front, const Vec3& up)
    {
        return glm::lookAt(pos, pos + front, up);
    }

    inline Mat4 ViewMatrix(const Vec3& pos, const Vec3& rot)
    {
        return glm::lookAt(pos, pos + FrontCamera(rot), Vec3{0.f, 1.f, 0.f});
    }

    Mat4 CubemapMatrix(const Vec3& pos, const Vec3& rot);

    template <typename... Args>
    auto Sum(Args&&... args)
    {
        return (args + ...);
    }

    template <typename... Args>
    auto Mean(Args&&... args)
    {
        return Sum(args...) / (sizeof...(args));
    }

    /** @fn GaussianFilter
     *
     * @brief Creates kernel for Gaussian blur
     */
    Matrix GaussianFilter(int height, int width, double sigma);


    /** @fn GaussianSmoothing(Image& image)
     *
     * @brief Blurring an image with four parallel passes.
     *
     * @param image Input image
     * @return Blurred image
     */
    [[nodiscard]] Image GaussianSmoothing(Image& image);
} // namespace Math
