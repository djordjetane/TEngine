#pragma once
#include "Components/Component.h"

namespace Entities {
    class Entity
    {
    public:
        [[nodiscard]] Uint GetId() const { return m_Id; }

    protected:
        static Uint ms_CurrentId;
        Uint m_Id;
        std::vector<std::unique_ptr<Component::IComponent>> m_Components;

    public:
        Entity() { m_Id = ms_CurrentId++; }

        template <typename TComponent>
        TComponent* GetComponent() const
        {
            for(auto& component : m_Components)
            {
                if(component->m_TypeId == Component::IComponent::GetComponentTypeID<TComponent>())
                {
                    return static_cast<TComponent*>(component.get());
                }
            }

            return nullptr;
        }

        template <typename TComponent>
        bool HasComponent()
        {
            for(auto& component : m_Components)
            {
                if(component->m_TypeId == Component::IComponent::GetComponentTypeID<TComponent>())
                {
                    return true;
                }
            }

            return false;
        }

        template <typename... TComponent>
        bool HasComponents()
        {
            if((HasComponent<TComponent>() && ...))
            {
                return true;
            }

            return false;
        }

        template <typename... TComponent>
        bool HasAnyComponents()
        {
            if((HasComponent<TComponent>() || ...))
            {
                return true;
            }

            return false;
        }

        template <typename TComponent, typename... TArgs>
        TComponent& AddComponent(TArgs&&... mArgs_)
        {
            if(HasComponent<TComponent>())
            {
                LOG_ERROR("Attempting to add a component twice! Entity ID: {}, ComponentType: {}", m_Id,
                          Component::IComponent::GetComponentTypeID<TComponent>());
            }

            auto component = std::make_unique<TComponent>(std::forward<TArgs>(mArgs_)...);

            component->m_TypeId = Component::IComponent::GetComponentTypeID<TComponent>();

            m_Components.push_back(std::move(component));

            return *(static_cast<TComponent*>(m_Components.back().get()));
        }

        void RemoveAllComponents() { m_Components.clear(); }

        /* Groups */
        /*
        template <typename TComponent>
        TComponent* GetComponentGroup() const
        {
            for (auto& component : m_Components)
            {
                if (component->m_TypeId == Component::IComponent::GetComponentTypeID<TComponent>())
                {
                    return static_cast<TComponent*>(component.get());
                }
            }

            return nullptr;
        }
        */
    };
} // namespace Entities