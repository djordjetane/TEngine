#include "Game.h"

#include <GameEntities.h>

namespace Game {
    GameApp::GameApp() : Application()
    {
        m_Entities["PlayerCamera"] = TMakeUnique<PlayerCamera>();
        m_Entities["Player"]       = TMakeUnique<Player>();
        m_Entities["Sky"]          = TMakeUnique<Sky>();
    }

    GameApp::~GameApp() = default;


    bool GameApp::GameInit()
    {
        auto* camera = static_cast<PlayerCamera*>(m_Entities["PlayerCamera"].get());
        SMASSERT(camera->Init(m_EntityManager), "Failed to init camera");

        auto* player = static_cast<Player*>(m_Entities["Player"].get());
        SMASSERT(player->Init(m_EntityManager, m_RenderSystem->GetShaders(), m_TextureManager), "Failed to init ball");

        auto* sky = static_cast<Sky*>(m_Entities["Sky"].get());
        SMASSERT(sky->Init(m_EntityManager, m_RenderSystem->GetShaders(), m_TextureManager), "Failed to init sky");

        LOG_INFO("Game initialized...");
        return true;
    }

    void GameApp::GameUpdate(float delta)
    {
        static_cast<PlayerCamera*>(m_Entities["PlayerCamera"].get())->Tick();
        static_cast<Player*>(m_Entities["Player"].get())->Tick();
        static_cast<Player*>(m_Entities["Sky"].get())->Tick();
    }

    bool GameApp::GameDestroy() { return true; }
} // namespace Game