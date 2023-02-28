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
        trans.Rotation = Vec3{0.f, -90.f, 0.f};

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
        auto  cameraTrans = camera->GetComponent<Component::Transformation>();

        cameraTrans->Rotation.y += 0.1f * minput->PositionOffset.x;
        cameraTrans->Rotation.x += 0.1f * minput->PositionOffset.y;

        cameraTrans->Rotation.x = glm::clamp(cameraTrans->Rotation.x, -1.f, 1.f);

        cameraTrans->Rotation.y = fmodf(cameraTrans->Rotation.y,  359.9f);
        
        auto front = Math::FrontCamera(cameraTrans->Rotation);
        
        //front.y = 0.f;
        front = Math::Normalize(front);
        Vec3 resultingVec{0.f,0.f,0.f};

        for(auto& e : input->Events)
            if(e.IsActive)
            {
                if(e.Action == "W")
                {
                    //move->Speed += front;
                    resultingVec += front;
                }
                if(e.Action == "S")
                {
                    //move->Speed -= front;
                    resultingVec -= front;
                }
                if(e.Action == "A")
                {
                    //move->Speed += Math::Normalize(Vec3{front.z, 0.f, -front.x});
                    resultingVec += Math::Normalize(Vec3{front.z, 0.f, -front.x});
                }
                if(e.Action == "D")
                {
                    //move->Speed += Math::Normalize(Vec3{-front.z, 0.f, front.x});
                    resultingVec += Math::Normalize(Vec3{-front.z, 0.f, front.x});
                }
            }

        
        if(Math::Length(resultingVec) > 1.f)
            resultingVec = Math::Normalize(resultingVec);
        move->Speed += resultingVec;
        

    }

} // namespace Game
