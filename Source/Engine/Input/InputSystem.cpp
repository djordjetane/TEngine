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

        //MouseStates.lastX        = MouseStates.X;
        //MouseStates.lastY        = MouseStates.Y;
        MouseStates.offset       = Vec2{0.f};
        MouseStates.scrollOffset = 0.f;
        /*
        for (auto *e : m_EntityManager->GetAllEntitiesWithComponent<Component::Input>())
        {
            auto *comp = e->GetComponent<Component::Input>();
            for (auto &event : comp->Events)
                event.IsActive = ActiveEvent(event.Action, event.ActionTriggerState);
            if (e->HasComponent<Component::Camera>())
            {
                
            }
        }
        */
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

        ms_States[APIKeyCode[key]] = action;

        ms_Mutex.unlock();
    }
#endif


    void InputSystem::mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        //float x_pos = static_cast<float>(xpos);
        //float y_pos = static_cast<float>(ypos);

        if (MouseStates.firstMouse)
        {
            MouseStates.offset     = Vec2{0.f};
            MouseStates.X          = xpos;
            MouseStates.Y          = ypos;
            MouseStates.firstMouse = false;
        }
        
        //float xoffset = (x_pos - MouseStates.lastX);
        //float yoffset = (MouseStates.lastY - y_pos); // reversed since y-coordinates go from bottom to top

        
        MouseStates.offset += Vec2(xpos - MouseStates.X, MouseStates.Y - ypos);
        MouseStates.X      = xpos;
        MouseStates.Y      = ypos;


        // CAMERA.UpdateMovement(xoffset, yoffset);
    }

    void InputSystem::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        MouseStates.scrollOffset += yoffset;
        // CAMERA.UpdateZoom(static_cast<float>(yoffset));
    }

    bool InputSystem::KeyPressed(const EKeyAction& key)
    {
        return ms_States[key] == EKeyEventState::PRESSED || ms_States[key] == EKeyEventState::REPEATED;
    }
} // namespace Input
