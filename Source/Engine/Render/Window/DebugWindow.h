#pragma once

namespace Render {
    class RenderSystem;
}
namespace Entities {
    class Entity;
    class EntityManager;
} // namespace Entities
using EntMan = Entities::EntityManager;

namespace Window {
    /* Wrapper for ImGui window */
    class DebugWindow
    {
    public:
        bool Init(NativeWindow* window, EntMan* entityManager = nullptr);
        bool Update();

        void Destroy();

        DebugWindow()  = default;
        ~DebugWindow() = default;

    private:
#ifdef _DEBUG
        NativeWindow* nativeWindow = nullptr;
        EntMan* m_EntityManager    = nullptr;
        void m_DrawGFXSettings() const;
        void m_DrawCameraMenu();
        // void m_DrawLightMenu() const;
        void m_DrawBallMenu() const;
#endif

        bool m_DebugMenuActive = true;
        bool m_ViewportsEnable = false;

        friend Render::RenderSystem;
    };
} // namespace Window
