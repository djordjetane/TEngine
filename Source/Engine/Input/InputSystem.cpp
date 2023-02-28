#include <precomp.h>
#include "InputSystem.h"

#include "Components/EntityComponent.h"


namespace Input {

    bool InputSystem::Init(Window::Window* window, Entities::EntityManager* entityManager)
    {
        SMASSERT(window != nullptr, "Failed to init window");
        SMASSERT(entityManager != nullptr, "Failed to init entMan");

        ms_Window     = window;
        auto* native = window->GetNativeWindow();

        ms_EntityManager = entityManager;
#ifdef _DEBUG
        glfwSetInputMode(native, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#else
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif //_DEBUG

        glfwSetKeyCallback(native, keyCallback);
        glfwSetMouseButtonCallback(native, mouseKeyCallback);
        glfwSetCursorPosCallback(native, mouseCallback);
        glfwSetScrollCallback(native, scrollCallback);

        InitKeys();
        InitAPIKeys();
        for(auto& p : KeyCode)
        {
            ms_States[p.first] = EKeyEventState::NONE;
        }

        LOG_INFO("Input system initialized");
        return true;
    }

    bool InputSystem::ActiveEvent(EKeyAction action, EKeyEventState state) { return ms_States[action] == state; }

    void InputSystem::ProcessInput(float delta)
    {
        glfwPollEvents();
        if(KeyPressed("Escape"))
        {
            ms_Window->Close();
        }
        
        for(auto* comp : ms_EntityManager->GetAllComponentInstances<Component::KeyInput>())
        {
            for (auto &event : comp->Events)
                event.IsActive = ActiveEvent(event.Action, event.ActionTriggerState);
        }
        for (auto *comp : ms_EntityManager->GetAllComponentInstances<Component::MInput>())
        {
            comp->PositionOffset = comp->Sensitivity * MouseStates.offset;
            comp->ScrollOffset   = MouseStates.scrollOffset;
        }
        
        MouseStates.offset       = Vec2{0.f};
        MouseStates.scrollOffset = 0.f;
    }

#ifdef GLFWAPI
    inline EKeyEventState API_glfwGetKey(GLFWwindow* window, EAPIKeyCode key)
    {
    	return static_cast<EKeyEventState>(glfwGetKey(window, static_cast<int>(key)));
    }
#endif
    void InputSystem::keyCallback(GLFWwindow* window, int _key, int _scancode, int _action, int _mod)
    {
        auto key    = static_cast<EAPIKeyCode>(_key);
        auto action = static_cast<EKeyEventState>(_action);
        
        ms_States[APIKeyCode[key]] = action;
    }


    void InputSystem::mouseKeyCallback(GLFWwindow *window, int _key, int _action, int _mods)
    {
        ms_States[APIKeyCode[EAPIKeyCode{_key}]] = EKeyEventState{_action};
    }

    void InputSystem::mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (!KeyPressed("RMB"))
        {
            MouseStates.firstMouse = true;
            return;
        }

        if (MouseStates.firstMouse)
        {
            MouseStates.offset     = Vec2{0.f};
            MouseStates.X          = xpos;
            MouseStates.Y          = ypos;
            MouseStates.firstMouse = false;
        }

        MouseStates.offset += Vec2(xpos - MouseStates.X, MouseStates.Y - ypos);
        MouseStates.X      = xpos;
        MouseStates.Y      = ypos;

    }

    void InputSystem::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        MouseStates.scrollOffset += yoffset;
    }

    bool InputSystem::KeyPressed(const EKeyAction& key)
    {
        return ms_States[key] == EKeyEventState::PRESSED || ms_States[key] == EKeyEventState::REPEATED;
    }
} // namespace Input
