#include <precomp.h>
#include "EntityManager.h"


namespace Entities {

    Uint Entity::ms_CurrentId = 0;

    EntityManager::~EntityManager()
    {
        for(auto& e : m_Table)
            e->RemoveAllComponents();
    }

    Entity* EntityManager::NewEntity()
    {
        m_Table.push_back(TMakeUnique<Entity>());
        return m_Table.back().get();
    }

    bool EntityManager::LoadEntity(Entity& entity)
    {
        m_Table.emplace_back(new Entity{std::move(entity)});
        return true;
    }

    bool EntityManager::LoadEntity(TUniquePtr<Entity>&& entity)
    {
        if(!entity)
        {
            LOG_FATAL("Entity manager failed to load entity");
            return false;
        }

        m_Table.push_back(std::move(entity));
        return true;
    }

    bool EntityManager::LoadEntity(Entity* entity)
    {
        m_Table.emplace_back(entity);
        return m_Table.back() != nullptr;
    }

    Entity* EntityManager::GetEntityById(Uint id) const { return id < m_Table.size() ? m_Table[id].get() : nullptr; }
} // namespace Entities