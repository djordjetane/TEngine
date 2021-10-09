#pragma once
#include "IEntity.h"

namespace Game {
    class PlayerCamera : public IEntity
    {
    public:
        PlayerCamera();
        bool Init(Engine::EntityManager* em);
        void Tick();

    private:
        Engine::EntityManager* m_EntityManager = nullptr;
    };
} // namespace Game
