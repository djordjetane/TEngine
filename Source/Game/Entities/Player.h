#pragma once
#include "IEntity.h"

namespace Game {
    class Player : public IEntity
    {
    public:
        Player() = default;
        bool Init(Engine::EntityManager* em, Engine::ShaderManager* sm, Engine::TextureManager* tm);
        void Tick();

    private:
        Engine::EntityManager* m_EntityManager   = nullptr;
        Engine::ShaderManager* m_ShaderManager   = nullptr;
        Engine::TextureManager* m_TextureManager = nullptr;
    };
} // namespace Game
