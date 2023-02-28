#include <precomp.h>
#include "Application.h"

namespace Engine {

    bool Application::Init()
    {
        m_EntityManager = new Entities::EntityManager();

        // Render system initialize
        m_RenderSystem = new Render::RenderSystem();
        SASSERT(m_RenderSystem->Init(m_EntityManager));


        // Texture Manager create
        m_TextureManager = new Texture::TextureManager;

        // Input System initialize
        m_InputSystem = new Input::InputSystem();
        SASSERT(m_InputSystem->Init(m_RenderSystem->GetWindow(), m_EntityManager));


        // Physics system initialize

        m_PhysicsSystem = new Physics::PhysicsSystem;
        if(!m_PhysicsSystem->Init(m_EntityManager))
        {
            LOG_FATAL("Failed to initialize PhysicsSystem");
            return false;
        }


        SMASSERT(GameInit(), "Error initializing game specific systems!");


        return true;
    }

    bool Application::Destroy()
    {
        GameDestroy();

        delete m_TextureManager;
        delete m_EntityManager;
        delete m_InputSystem;
        delete m_RenderSystem;

        return true;
    }

    Entities::EntityManager* Application::GetEntityManager() const { return m_EntityManager; }

    Texture::TextureManager* Application::GetTextureManager() const { return m_TextureManager; }

    void Application::Run()
    {
        m_running   = true;
        float delta = static_cast<float>(glfwGetTime());

        // Main loop
        while(m_RenderSystem->IsRunning())
        {

            delta = static_cast<float>(glfwGetTime()) - delta;
            Update(delta);
        }

        m_running = false;
    }

    void Application::Update(float delta)
    {
        // Update all systems
        m_InputSystem->ProcessInput(delta);
        m_PhysicsSystem->Update(delta);
        m_RenderSystem->Update();

        GameUpdate(delta);
    }
} // namespace Engine