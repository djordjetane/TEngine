#pragma once
#include <CoreGame.h>

namespace Game {
    struct IEntity;

    class GameApp final : public Engine::Application
    {
    public:
        bool GameInit() override;
        void GameUpdate(float delta) override;
        bool GameDestroy() override;

    private:
        THashMap<String, TUniquePtr<IEntity>> m_Entities;
    };

} // namespace Game

inline Engine::Application* Engine::NewApp() { return new Game::GameApp(); }