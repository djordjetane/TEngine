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

        inline static struct InputMouseStates
        {
            double X                = WindowData::W / 2.0f;
            double Y                = WindowData::H / 2.0f;
            //float lastX            = WindowData::W / 2.0f;
            //float lastY            = WindowData::H / 2.0f;
            Vec2  offset{0.f};
            bool  firstMouse       = true;
            double scrollOffset     = 0.f;
            double MouseSensitivity = 0.1f;
        } MouseStates;

        static const Vec2& GetMouseOffset()
        {
            return MouseStates.offset;
            //return Vec2((MouseStates.X - MouseStates.lastX), (MouseStates.lastY - MouseStates.Y));
        }
        /*
        inline static float X         = WindowData::W / 2.0f;
        inline static float Y         = WindowData::H / 2.0f;
        inline static float lastX     = WindowData::W / 2.0f;
        inline static float lastY     = WindowData::H / 2.0f;
        inline static bool firstMouse = true;
        inline static float yOffset    = 0.f;
        inline static float MouseSensitivity = 0.1f;
        */

        inline static void keyCallback(GLFWwindow*window, int _key, int _scancode, int _action, int _mod);
        inline static void mouseCallback(GLFWwindow*window, double xpos, double ypos);
        inline static void scrollCallback(GLFWwindow*window, double xoffset, double yoffset);

        // static bool KeyPressed(int key);
        static bool KeyPressed(const EKeyAction& key);
        static bool ActiveEvent(EKeyAction action, EKeyEventState state);

    private:
        inline static Window::Window* ms_Window   = nullptr;
        inline static Entities::EntityManager* ms_EntityManager = nullptr;

        inline static THashMap<EKeyAction, EKeyEventState> ms_States{};
        inline static Mutex ms_Mutex;
    };
} // namespace Input
