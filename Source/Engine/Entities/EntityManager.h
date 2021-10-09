#pragma once

#include <Entities/Entity.h>

#ifdef _DEBUG
namespace Window {
    class DebugWindow;
}
#endif

namespace Entities {
    class EntityManager
    {
    public:
        EntityManager() = default;
        ~EntityManager();
        EntityManager(const EntityManager& other) = delete;
        EntityManager& operator=(EntityManager&& other) = delete;

        Entity* NewEntity();

        bool LoadEntity(Entity& entity);
        bool LoadEntity(TUniquePtr<Entity>&& entity);
        bool LoadEntity(Entity* entity);

        Entity* GetEntityById(Uint id);

        template <typename TComponent>
        auto GetAllEntitiesWithComponent()
        {
            std::vector<Entity*> returnVec{};

            for(auto& entity : m_Table)
            {
                if(entity->HasComponent<TComponent>())
                {
                    returnVec.push_back(entity.get());
                }
            }

            return returnVec;
        }

        template <typename... TComponent>
        auto GetEntitiesWithComponents()
        {
            std::vector<Entity*> returnVec{};
            for(auto& entity : m_Table)
            {
                if(entity.get()->HasComponents<TComponent...>())
                    returnVec.push_back(entity.get());
            }

            return returnVec;
        }

        template <typename... TComponent>
        auto GetEntitiesWithAnyComponents()
        {
            std::vector<Entity*> returnVec{};
            for(auto& entity : m_Table)
            {
                if(entity.get()->HasAnyComponents<TComponent...>())
                    returnVec.push_back(entity.get());
            }

            return returnVec;
        }

        template <typename TComponent>
        auto GetAllComponentInstances()
        {
            std::vector<TComponent*> returnVec{};

            for(auto& entity : m_Table)
            {
                if(TComponent* component = entity->GetComponent<TComponent>())
                {
                    returnVec.push_back(component);
                }
            }

            return returnVec;
        }

    private:
        std::vector<std::unique_ptr<Entity>> m_Table{};
#ifdef _DEBUG
        friend Window::DebugWindow;
#endif
    };
} // namespace Entities
