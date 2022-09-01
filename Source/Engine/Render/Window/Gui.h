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
    class Gui
    {
    public:
        bool Init(NativeWindow* window, EntMan* entityManager = nullptr);
        bool Update();

        void Destroy();

    private:

        NativeWindow* nativeWindow = nullptr;
        EntMan* m_EntityManager    = nullptr;
        void mDrawGFXSettings() const;
        void mDrawEntityList() const;
        //void m_DrawCameraMenu();
        // void m_DrawLightMenu() const;

        bool m_GuiActive = true;
        bool m_ViewportsEnable = false;

        friend Render::RenderSystem;
    };
} // namespace Window
