#include <precomp.h>

namespace Window {
    bool Window::Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, WindowData::majVer);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, WindowData::minVer);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_nativeWindow = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, WindowData::title, nullptr, nullptr);
        SMASSERT(m_nativeWindow != nullptr, "Failed to init glfwWindow");

        glfwMakeContextCurrent(m_nativeWindow);
        glfwSwapInterval(WindowData::vsync ? 1 : 0);

        m_running = true;

        return true;
    }

    bool Window::IsRunning() const { return m_running; }

    void Window::Update()
    {
        glfwSwapInterval(WindowData::vsync ? 1 : 0);

        if(glfwWindowShouldClose(m_nativeWindow) == 0)
        {
            glfwSwapBuffers(m_nativeWindow);
            return;
        }

        m_running = false;
    }

    GLFWwindow* Window::GetNativeWindow() const { return m_nativeWindow; }

    void Window::Close()
    {
        glfwSetWindowShouldClose(m_nativeWindow, 1);
        m_running = false;
    }

    void Window::Destroy()
    {
        glfwDestroyWindow(m_nativeWindow);
        m_running = false;
        LOG_INFO("Window closed...");
    }

    void Window::FrameBufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
    {
        glViewport(0, 0, width, height);
        WindowData::Update(width, height);
    }
} // namespace Window
