#include <precomp.h>
#include "RenderSystem.h"
#include "Components/Components.h"
#include "Renderer.h"
#include "Window/Gui.h"

bool Render::RenderSystem::Init(Entities::EntityManager* entityManager)
{
    m_EntityManager                            = entityManager;
    m_Renderer                                 = new Renderer;
    m_Renderer->m_Gui                  = new Window::Gui;
    m_Renderer->m_Gui->m_EntityManager = entityManager;

    SMASSERT(m_Renderer->Init(), "Failed to init RenderSystem");

    LOG_INFO("RenderSystem initialized");

    return true;
}

void Render::RenderSystem::Update() const
{
    if(!m_Renderer->IsRunning())
        return;

    m_Renderer->StartScene();

    auto cameras(m_EntityManager->GetEntitiesWithComponents<Component::Camera>());
    if(cameras.empty() || cameras[0] == nullptr)
    {
        LOG_FATAL("Must have at least one camera");
        m_Renderer->m_Window->Destroy();
        return;
    }

    auto renderables{m_EntityManager->GetEntitiesWithAnyComponents<Component::Material, Component::Sprite>()};

    for (auto *camera : cameras)
    {
        m_Renderer->DrawEntities(renderables, camera);

        auto cubeMap{m_EntityManager->GetEntitiesWithComponents<Component::Sky>().at(0)};
        if (cubeMap != nullptr)
            m_Renderer->DrawCubeMap(cubeMap, camera);
    }
    m_Renderer->EndScene();
}

bool Render::RenderSystem::IsRunning() const { return m_Renderer->m_Window->IsRunning(); }

Window::Window* Render::RenderSystem::GetWindow() const { return m_Renderer->m_Window; }

Render::ShaderManager* Render::RenderSystem::GetShaders() const { return m_Renderer->m_ShaderManager; }

Render::RenderSystem::~RenderSystem()
{
	delete m_Renderer;
}
