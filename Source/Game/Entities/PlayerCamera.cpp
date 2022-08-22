#include "PlayerCamera.h"

namespace Game {
    PlayerCamera::PlayerCamera() : IEntity() {}

    bool PlayerCamera::Init(Engine::EntityManager* em)
    {
        m_EntityManager      = em;
        using Transformation = Component::Transformation;


        /* Camera */
        auto entity = TMakeUnique<Entities::Entity>();
        
        auto& trans = entity->AddComponent<Transformation>();
        trans.Position.z = 10.f;
        trans.Rotation = Vec3{0.f, 0.f, -90.f};

        entity->AddComponent<Component::MInput>();
        entity->AddComponent<Component::Camera>();

        entity->AddComponent<Component::KeyInput>().Events = {
            KeyEvent{"W"}, KeyEvent{"W", Input::EKeyEventState::REPEATED},
            KeyEvent{"S"}, KeyEvent{"S", Input::EKeyEventState::REPEATED},
            KeyEvent{"A"}, KeyEvent{"A", Input::EKeyEventState::REPEATED},
            KeyEvent{"D"}, KeyEvent{"D", Input::EKeyEventState::REPEATED},
        };

        entity->AddComponent<Component::Movement>().friction = 0.4f;

        ID = entity->GetId();

        return m_EntityManager->LoadEntity(std::move(entity));
    }

    void PlayerCamera::Tick()
    {
        // Camera
        auto* camera = m_EntityManager->GetEntityById(ID);

        auto* move  = camera->GetComponent<Component::Movement>();
        auto* input = camera->GetComponent<Component::KeyInput>();
        auto* minput = camera->GetComponent<Component::MInput>();

        for(auto& e : input->Events)
            if(e.IsActive)
            {
                if(e.Action == "W")
                {
                    move->Speed.z = -1.f;
                }
                if(e.Action == "S")
                {
                    move->Speed.z = 1.f;
                }
                if(e.Action == "A")
                {
                    move->Speed.x = -1.f;
                }
                if(e.Action == "D")
                {
                    move->Speed.x = 1.f;
                }

            }
        auto cameraTrans = camera->GetComponent<Component::Transformation>();

        constexpr float constrainPitchBound  = 89.f;

        cameraTrans->Rotation.z += minput->PositionOffset.x;
        cameraTrans->Rotation.x += minput->PositionOffset.y;

        cameraTrans->Rotation.x = std::max(-constrainPitchBound, cameraTrans->Rotation.x);
        cameraTrans->Rotation.x = std::min(cameraTrans->Rotation.x, constrainPitchBound);
    }

} // namespace Game
