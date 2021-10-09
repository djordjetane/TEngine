#pragma once
#include "InputEvent.h"

namespace Entities {
    class EntityManager;
}
namespace Input {
    class InputSystem
    {
    public:
        InputSystem() = default;
        bool Init(Window::Window* window, Entities::EntityManager* entityManager);
        void ProcessInput(float delta);

        inline static float lastX     = WindowData::W / 2.0f;
        inline static float lastY     = WindowData::H / 2.0f;
        inline static bool firstMouse = true;

        inline static void keyCallback(GLFWwindow* window, int _key, int _scancode, int _action, int _mod);
        inline static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        inline static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        // static bool KeyPressed(int key);
        static bool KeyPressed(const EKeyAction& key);
        static bool ActiveEvent(EKeyAction action, EKeyEventState state);

    private:
        inline static Window::Window* m_Window   = nullptr;
        Entities::EntityManager* m_EntityManager = nullptr;

        inline static THashMap<EKeyAction, EKeyEventState> m_States{};
        inline static Mutex ms_Mutex;
    };
} // namespace Input
