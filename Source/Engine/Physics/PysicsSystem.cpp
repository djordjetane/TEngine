#include <precomp.h>
#include "PysicsSystem.h"


namespace Physics {
    bool PhysicsSystem::Init(Entities::EntityManager* entityManager)
    {
        m_EntityManager = entityManager;

        return true;
    }

    void PhysicsSystem::Update(float delta) { m_UpdateMovements(); }

    void PhysicsSystem::Shutdown() {}

    void PhysicsSystem::m_UpdateMovements() const
    {
        using Transformation = Component::Transformation;
        using Movement       = Component::Movement;
        auto moveables = m_EntityManager->GetEntitiesWithComponents<Transformation, Movement>();
        for(auto& e : moveables)
        {
            auto* transf   = e->GetComponent<Transformation>();
            auto* movement = e->GetComponent<Movement>();

            transf->Position += movement->Speed;
            transf->Rotation += movement->Rotation;


            if(Math::Length(movement->Speed) > 0.f)
                movement->Speed -= movement->friction * movement->Speed;
            if(Math::Length(movement->Rotation) > 0.f)
                movement->Rotation -= movement->friction * movement->Rotation;
        }
    }


} // namespace Physics
