#pragma once

namespace Render {
    class RenderSystem;
}
namespace Input {
    class InputSystem;
}
namespace Entities {
    class EntityManager;
}
namespace Texture {
    class TextureManager;
}

// class PhysicsSystem;

namespace Engine {
    class Application
    {
    public:
        Application()                   = default;
        virtual ~Application()          = default;
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&)                 = delete;

        bool Init();
        void Run();
        bool Destroy();

        [[nodiscard]]Entities::EntityManager* GetEntityManager() const;
        [[nodiscard]]Texture::TextureManager* GetTextureManager() const;

    protected:
        Entities::EntityManager* m_EntityManager  = nullptr;
        Render::RenderSystem* m_RenderSystem      = nullptr;
        Input::InputSystem* m_InputSystem         = nullptr;
        Texture::TextureManager* m_TextureManager = nullptr;
        Physics::PhysicsSystem* m_PhysicsSystem   = nullptr;
        void Update(float delta);

        virtual bool GameInit()              = 0;
        virtual bool GameDestroy()           = 0;
        virtual void GameUpdate(float delta) = 0;

    private:
        bool m_running = false;
    };
    extern Application* NewApp();
} // namespace Engine