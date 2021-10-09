#pragma once

namespace Math {
    /* Types */
    using Array = TArray<float>;
    using Matrix = TArray<Array>;

    /* Constants */
    constexpr double Pi = 3.14159265359;
    constexpr mat4 I4{1.f};
    constexpr vec3 e3{1.f};
    constexpr vec3 v3{1.f, 1.f, 1.f};
    constexpr vec3 NullVec2{0.f};
    constexpr vec3 NullVec3{0.f};

    inline glm::vec3 NormalOnDirection(const glm::vec3& direction)
    {
        return glm::normalize(glm::cross(direction, {0.f, 1.f, 0.f}));
    }

    template <typename T>
    double Gaussian(T x, T y, double sigma)
    {
        return exp(-(x * x + y * y) / (2.f * sigma * sigma)) / (2.f * Pi * sigma * sigma);
    }

    inline mat4 Translate(mat4& m, vec3& v) { return glm::translate(m, v); }
    inline mat4 Translation(vec3& v) { return glm::translate(I4, v); }
    inline mat4 Rotate(mat4& m, float angle, vec3& v) { return glm::rotate(m, angle, v); }
    inline mat4 Rotation(float angle, vec3& v) { return glm::rotate(I4, angle, v); }
    inline mat4 Scale(mat4& m, vec3& scale) { return glm::scale(m, scale); }
    inline mat4 Scale(mat4& m, float scale) { return glm::scale(m, scale * e3); }

    /* Eulerian angles */
    /** 
     * @brief Rotation matrix from eulerian angles in degrees
     * @param x Roll around x-axis
     * @param y Pitch around y-axis
     * @param z Yaw around z-axis
     *
     * @return 4x4 rotation matrix
     */
    inline mat4 EulerAnglesRotate(float x, float y, float z)
    {
        float sinx = sinf(x), cosx = cosf(x);
        float siny = sinf(y), cosy = cosf(y);
        float sinz = sinf(z), cosz = cosf(z);
        return mat4{
            cosz*cosy,      cosz*siny*sinx - sinz*cosx,     cosz*siny*cosx + sinz*sinx,     0.f,
            sinz*cosy,      sinz*siny*sinx - cosz*cosx,     sinz*siny*cosx - cosz*sinx,     0.f,
            -siny,              cosy*sinx,                          cosy*cosx,              0.f,
             0.f,                   0.f,                                0.f,                1.f
        };
    }

    /**
     * @see EulerAnglesRotate(float x, float y, float z)
     */
    inline mat4 EulerAnglesRotate(vec3& angles)
    {
        angles = std::move( glm::radians<3, float, glm::qualifier::defaultp>(angles) );
        return EulerAnglesRotate(angles.x, angles.y, angles.z);
    }
    
    // Translate * Rotate * Scale
    inline mat4 ModelMatrix(vec3& position, vec3& rotation, vec3& scale)
    {
        return Scale( EulerAnglesRotate(rotation).operator*=( Translation(position) ), scale);
    }

    inline mat4 OrthoModelMatrix(const vec2& position, float rotation, const vec2& scale)
    {

        mat4 model = glm::translate(Math::I4, vec3(position, 0.f));

            
        model = Translate(model, vec3{0.5f * scale.x, 0.5f * scale.y, 0.0f}); 

        model = Rotate(model, glm::radians(rotation), vec3{0.0f, 0.0f, 1.0f}); 
        model = Translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
        return  Scale(model, glm::vec3(scale, 1.0f));
    }

    inline mat4 ViewMatirx(const vec3& pos, const vec3& front, const vec3& up)
    {
        return glm::lookAt(pos, front, up);
    }

    template<typename... Args>
    auto Sum(Args&&... args)
    {
        return (args + ...);
    }

    template<typename... Args>
    auto Mean(Args&&... args)
    {
        return Sum(args...)/( sizeof ... (args) );
    }

    [[nodiscard]] inline Matrix GaussianFilter(int height, int width, double sigma)
    {
        Matrix kernel(height, Array(width));
        float sum = 0.0;

        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
            {
                kernel[i][j] = static_cast<float>(Gaussian(i, j, sigma));
                sum += kernel[i][j];
            }

        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
                kernel[i][j] /= sum;

        return kernel;
    }


    inline Image ApplyFilterParallel(Image& im, Image& filter, bool hor)
    {
        size_t n = hor ? im.size() - filter.size() + 1 : im[0].size() - filter[0].size() + 1;
        size_t m = hor ? im[0].size() - filter[0].size() + 1 : im.size() - filter.size() + 1;
        Image newImage(n, Array(m, 0.f));
        for(size_t i = 0; i < n; i++)
            for(size_t j = 0; j < m; j++)
                for(int h = i; h < i + filter.size(); h++)
                    for(int w = j; w < j + filter[0].size(); w++)
                        newImage[i][j] += filter[h - i][w - j] * im[h][w];

        return newImage;
    }


    [[nodiscard]] inline Image GaussianSmoothing(Image& image)
    {
        Image filter{GaussianFilter(5, 5, 50.f)};
        Image rotated(image.size(), std::vector<float>(image[0].size()));
        std::reverse_copy(image.begin(), image.end(), rotated.begin());


        auto a1 = std::async(std::launch::async, [&] { return ApplyFilterParallel(image, filter, true); });
        auto a2 = std::async(std::launch::async, [&] { return ApplyFilterParallel(image, filter, false); });
        auto a3 = std::async(std::launch::async, [&] { return ApplyFilterParallel(rotated, filter, true); });
        auto a4 = std::async(std::launch::async, [&] { return ApplyFilterParallel(rotated, filter, false); });

        Image newImageH1 = a1.get();
        Image newImageV1 = a2.get();
        Image newImageH2 = a3.get();
        Image newImageV2 = a4.get();


        size_t newImgH = newImageV1.size();
        size_t newImgW  = newImageV1[0].size();
        image.resize(newImgH, Array(newImgW, 0));

        for(size_t i = 0; i < newImgH; i++)
            for(size_t j = 0; j < newImgW; j++)
                image[i][j] = Mean(newImageH1[i][j], newImageV1[i][j], 
                             newImageH2[newImgH-i-1][newImgW-j-1], newImageV2[newImgH-i-1][newImgW-j-1]);

        return image;
    }
} // namespace Math
