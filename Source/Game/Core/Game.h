#pragma once
#include <CoreGame.h>

namespace Game {
    struct IEntity;

    class GameApp : public Engine::Application
    {
    public:
        GameApp();
        ~GameApp() override;
        bool GameInit() override;
        void GameUpdate(float delta) override;
        bool GameDestroy() override;

    private:
        THashMap<String, TUniquePtr<IEntity>> m_Entities;
    };

} // namespace Game

Engine::Application* Engine::NewApp() { return new Game::GameApp(); }