#pragma once
#include <CoreGame.h>

namespace Game {
    struct IEntity
    {
    protected:
        Uint ID;
    public:
        virtual void Tick() = 0;
    };
    inline void IEntity::Tick()
    {
    }

} // namespace Game
