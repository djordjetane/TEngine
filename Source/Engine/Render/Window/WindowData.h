#pragma once

struct WindowData
{
    inline static int width                   = 1280;
    inline static int height                  = 720;
    inline static constexpr char* title       = "GolfGL";
    inline static constexpr char* glslVersion = "#version 330";
    inline static bool fxaa                   = true;
    inline static bool vsync                  = true;

    // Just to avoid massive number of casting
    inline static float W = 1280.f;
    inline static float H = 720.f;

    inline static void Update(int newWidth, int newHeight)
    {
        width  = newWidth;
        W      = static_cast<float>(newWidth);
        height = newHeight;
        H      = static_cast<float>(newHeight);
    }


    inline static String GetResolution() { return std::to_string(width).append("x").append(std::to_string(height)); }

    inline static float GetAspect() { return W / H; }
};

#define WND_WIDTH (WindowData::width)
#define WND_HEIGHT (WindowData::height)