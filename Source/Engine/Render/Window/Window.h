#pragma once

namespace Render {
    class Renderer;
}

namespace Window {
    class Window
    {
    public:
        Window(const Window&) = delete;
        Window(Window&&)      = delete;
        void operator=(const Window&) = delete;
        Window()                      = default;

        NativeWindow* GetNativeWindow() const;

        bool Init();
        bool IsRunning() const;
        void Update();

        void Close();
        void Destroy();

        static void FrameBufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);

    private:
        NativeWindow* m_nativeWindow = nullptr;
        bool m_running               = false;
    };
} // namespace Window
