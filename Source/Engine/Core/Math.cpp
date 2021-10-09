#include <precomp.h>
#include "Math.h"


namespace Math {

    Mat4 EulerAnglesRotate(float x, float y, float z)
    {
        float sinx = sinf(x), cosx = cosf(x);
        float siny = sinf(y), cosy = cosf(y);
        float sinz = sinf(z), cosz = cosf(z);
        return Mat4{cosz * cosy,
                    cosz * siny * sinx - sinz * cosx,
                    cosz * siny * cosx + sinz * sinx,
                    0.f,
                    sinz * cosy,
                    sinz * siny * sinx - cosz * cosx,
                    sinz * siny * cosx - cosz * sinx,
                    0.f,
                    -siny,
                    cosy * sinx,
                    cosy * cosx,
                    0.f,
                    0.f,
                    0.f,
                    0.f,
                    1.f};
    }

    Mat4 EulerAnglesRotate(const Vec3& degrees)
    {
        auto v{glm::radians<3, float, glm::qualifier::packed_highp>(degrees)};
        return EulerAnglesRotate(v.x, v.y, v.z);
    }

    Mat4 ModelMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale)
    {
        return Scale(Translation(position) * EulerAnglesRotate(rotation), scale);
    }

    Mat4 OrthoModelMatrix(const Vec2& position, float rotation, const Vec2& scale)
    {
        Mat4 model = Translate(Mat4{Math::I4}, Vec3{position, 0.f});
        model      = Translate(model, Vec3{0.5f * scale.x, 0.5f * scale.y, 0.0f});
        model      = Rotate(model, glm::radians(rotation), Vec3{0.0f, 0.0f, 1.0f});
        model      = Translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
        return Scale(model, glm::vec3(scale, 1.0f));
    }

    Mat4 CubemapMatrix(const Vec3& pos, const Vec3& rot)
    {
        auto view{ Math::ViewMatrix(pos, rot) };
        view[0][3] = 0.f;
        view[1][3] = 0.f;
        view[2][3] = 0.f;
        view[3][3] = 1.f;

        view[3][0] = 0.f;
        view[3][1] = 0.f;
        view[3][2] = 0.f;

        return view;
    }

    Matrix GaussianFilter(int height, int width, double sigma)
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


    Image ApplyFilterParallel(Image& im, Image& filter, bool hor)
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


    Image GaussianSmoothing(Image& image)
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
        size_t newImgW = newImageV1[0].size();
        image.resize(newImgH, Array(newImgW, 0));

        for(size_t i = 0; i < newImgH; i++)
            for(size_t j = 0; j < newImgW; j++)
                image[i][j] = Mean(newImageH1[i][j], newImageV1[i][j], newImageH2[newImgH - i - 1][newImgW - j - 1],
                                   newImageV2[newImgH - i - 1][newImgW - j - 1]);

        return image;
    }
} // namespace Math
