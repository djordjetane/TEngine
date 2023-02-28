#pragma once

namespace Entities {
    class EntityManager;
}

namespace Render {
    class Renderer;

    class RenderSystem
    {
    public:
        RenderSystem(const RenderSystem&) = delete;
        RenderSystem(RenderSystem&&)      = delete;
        void operator=(const RenderSystem&) = delete;

        bool Init(Entities::EntityManager* entityManager);
        void Update() const;

        [[nodiscard]] bool IsRunning() const;

        [[nodiscard]] Window::Window* GetWindow() const;
        [[nodiscard]] ShaderManager* GetShaders() const;

        RenderSystem() = default;
        ~RenderSystem();

    private:
        Renderer* m_Renderer                     = nullptr;
        Entities::EntityManager* m_EntityManager = nullptr;
    };
} // namespace Render
