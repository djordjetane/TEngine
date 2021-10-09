#include <precomp.h>
#include "InputSystem.h"

#include "Components/EntityComponent.h"


namespace Input {

    bool InputSystem::Init(Window::Window* window, Entities::EntityManager* entityManager)
    {
        SMASSERT(window != nullptr, "Failed to init window");
        SMASSERT(entityManager != nullptr, "Failed to init entMan");

        m_Window     = window;
        auto* native = window->GetNativeWindow();

        m_EntityManager = entityManager;
#ifdef _DEBUG
        glfwSetInputMode(native, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#else
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif //_DEBUG

        glfwSetKeyCallback(native, keyCallback);
        glfwSetCursorPosCallback(native, mouseCallback);
        glfwSetScrollCallback(native, scrollCallback);

        InitKeys();
        InitAPIKeys();
        for(auto& p : KeyCode)
        {
            m_States[p.first] = EKeyEventState::NONE;
        }

        LOG_INFO("Input system initialized");
        return true;
    }

    bool InputSystem::ActiveEvent(EKeyAction action, EKeyEventState state) { return m_States[action] == state; }

    void InputSystem::ProcessInput(float delta)
    {
        glfwPollEvents();
        if(KeyPressed("Escape"))
        {
            m_Window->Close();
        }

        for(auto* comp : m_EntityManager->GetAllComponentInstances<Component::Input>())
            for(auto& event : comp->Events)
                event.IsActive = ActiveEvent(event.Action, event.ActionTriggerState);

        /*
        CAMERA.SetCameraSpeed(deltaTime);

        // W
        if(KEY_PRESSED(GLFW_KEY_W))
        {
            CAMERA.LookUp();
        }
        // S
        if(KEY_PRESSED(GLFW_KEY_S))
        {
            CAMERA.LookDown();
        }
        // D
        if(KEY_PRESSED(GLFW_KEY_D))
        {
            CAMERA.LookRight();
        }
        // A
        if(KEY_PRESSED(GLFW_KEY_A))
        {
            CAMERA.LookLeft();
        }
        */
    }

#ifdef GLFWAPI
    inline EKeyEventState API_glfwGetKey(GLFWwindow* window, EAPIKeyCode key)
    {
        return static_cast<EKeyEventState>(glfwGetKey(window, static_cast<int>(key)));
    }

    void InputSystem::keyCallback(GLFWwindow* window, int _key, int _scancode, int _action, int _mod)
    {
        auto key    = static_cast<EAPIKeyCode>(_key);
        auto action = static_cast<EKeyEventState>(_action);

        ms_Mutex.lock();

        m_States[APIKeyCode[key]] = action;

        ms_Mutex.unlock();
    }
#endif


    void InputSystem::mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        float x_pos = static_cast<float>(xpos);
        float y_pos = static_cast<float>(ypos);

        if(InputSystem::firstMouse)
        {
            InputSystem::lastX      = x_pos;
            InputSystem::lastY      = y_pos;
            InputSystem::firstMouse = false;
        }

        float xoffset = x_pos - InputSystem::lastX;
        float yoffset = InputSystem::lastY - y_pos; // reversed since y-coordinates go from bottom to top

        InputSystem::lastX = x_pos;
        InputSystem::lastY = y_pos;

        // CAMERA.ProcessMouseMovement(xoffset, yoffset);
    }

    void InputSystem::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        // CAMERA.ProcessMouseScroll(static_cast<float>(yoffset));
    }

    bool InputSystem::KeyPressed(const EKeyAction& key)
    {
        return m_States[key] == EKeyEventState::PRESSED || m_States[key] == EKeyEventState::REPEATED;
    }
} // namespace Input
