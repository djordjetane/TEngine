#include <GameEntities.h>
#include "Game.h"


namespace Game {
    bool GameApp::GameInit()
    {
        auto camera = TMakeUnique<PlayerCamera>();
        SMASSERT(camera->Init(m_EntityManager), "Failed to init camera");
        m_Entities["PlayerCamera"] = std::move(camera);

        //TUniquePtr<Player> player{};
        auto player = TMakeUnique<Player>();
        SMASSERT(player->Init(m_EntityManager, m_RenderSystem->GetShaders(), m_TextureManager), "Failed to init ball");
        m_Entities["Player"] = std::move(player);

        auto sky = TMakeUnique<Sky>();
        SMASSERT(sky->Init(m_EntityManager, m_RenderSystem->GetShaders(), m_TextureManager), "Failed to init sky");
        m_Entities["Sky"] = std::move(sky);

        LOG_INFO("Game initialized...");
        return true;
    }

    void GameApp::GameUpdate(float delta)
    {
        for (auto &[k, e] : m_Entities)
            e->Tick();
    }

    bool GameApp::GameDestroy() { return true; }
} // namespace Game