#pragma once
#include "Vertex/VertexArray.h"


namespace Window {
    class Window;
	class Gui;
} // namespace Window

namespace Render {
    class Framebuffer;
    class ShaderManager;

    /** @class Renderer
     *
     * @brief Drawing scene
     */
    class Renderer
    {
    public:
        Renderer()                = default;
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&)      = delete;
        Renderer& operator=(const Renderer&) = delete;
        ~Renderer();

        /* Public interface */
        bool Init();
        bool IsRunning() const;
        void StartScene() const;
        void EndScene() const;

        /* Entities */
        void DrawEntities(std::vector<Entities::Entity*>& entities, Entities::Entity* camera) const;
        void DrawMaterial(Entities::Entity* entity, Entities::Entity* camera) const;
        void DrawSprite(Entities::Entity* entity, Entities::Entity* camera) const;
        void DrawCubeMap(Entities::Entity* entity, Entities::Entity* camera) const;

    private:
        // Windows
        Window::Window* m_Window = nullptr;
        Window::Gui* m_Gui = nullptr;

        Framebuffer* m_FrameBuff       = nullptr;
        ShaderManager* m_ShaderManager = nullptr;

        static void m_ClearColor();

        friend RenderSystem;
    };
} // namespace Render
