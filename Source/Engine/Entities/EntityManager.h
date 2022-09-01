#pragma once

#include <Entities/Entity.h>

namespace Engine {
    class Application;
}

namespace Window {
    class Gui;
}

namespace Entities {
    class EntityManager
    {
    public:
        ~EntityManager();
        EntityManager(const EntityManager& other) = delete;
        EntityManager& operator=(EntityManager&& other) = delete;

        Entity* NewEntity();

        bool LoadEntity(Entity& entity);
        bool LoadEntity(TUniquePtr<Entity>&& entity);
        bool LoadEntity(Entity* entity);

        [[nodiscard]] Entity* GetEntityById(Uint id) const;

        template <typename TComponent>
        auto GetAllEntitiesWithComponent() const
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
        auto GetEntitiesWithComponents() const
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
        auto GetEntitiesWithAnyComponents() const
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

        EntityManager() = default;

        friend Engine::Application;
    	friend Window::Gui;

    };
} // namespace Entities
