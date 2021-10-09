#pragma once

namespace Physics {
    class PhysicsSystem
    {
    public:
        PhysicsSystem()  = default;
        ~PhysicsSystem() = default;

        PhysicsSystem(const PhysicsSystem&) = delete;
        PhysicsSystem& operator=(const PhysicsSystem&) = delete;
        PhysicsSystem(PhysicsSystem&&)                 = delete;

        bool Init(Entities::EntityManager* entityManager);
        void Update(float delta);
        void Shutdown();

    private:
        Entities::EntityManager* m_EntityManager = nullptr;

        void m_UpdateMovements() const;
    };
} // namespace Physics