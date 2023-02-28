#include "Player.h"
#include <CoreGame.h>

#include "Builders/ObjImporter.h"


namespace Game {
    bool Player::Init(Engine::EntityManager* em, Engine::ShaderManager* sm, Engine::TextureManager* tm)
    {
        m_EntityManager  = em;
        m_ShaderManager  = sm;
        m_TextureManager = tm;

        using Transformation = Component::Transformation;
        using Material       = Component::Material;

        /* Ball */
        auto entity       = std::make_unique<Engine::Entity>();
        Material& matComp = entity->AddComponent<Material>();
        Engine::ObjImporter importer{"Resources/Objects/golfBall", "golfBall.obj"};
        importer.Import();
        auto model{importer.GetData().Meshes[0]};
        Render::BufferLayout layout{{Render::EShaderDataType::Float3, "aPosition"}, {Render::EShaderDataType::Float3, "aNormal"},
                                    {Render::EShaderDataType::Float2, "aTexCoords"}, {Render::EShaderDataType::Float3, "aTangent"},{Render::EShaderDataType::Float3, "aBitangent"}};
        Render::VertexBuffer* vBuffer  = new Render::VertexBuffer(model.Vertices, layout);
        Render::ElementBuffer* eBuffer = new Render::ElementBuffer(model.Indices);
        auto& mesh                     = entity->AddComponent<Component::Mesh>();
        mesh.VArray.AddVertexBuffer(vBuffer);
        mesh.VArray.SetElementBuffer(eBuffer);


        Texture::Metadata meta;
        meta.Channels    = 3;
        matComp.Textures = {m_TextureManager->LoadExternalTexture("Resources/Objects/golfBall", "albedo.png", meta),
                            m_TextureManager->LoadExternalTexture("Resources/Objects/golfBall", "normal.png", meta),
                            m_TextureManager->LoadExternalTexture("Resources/Objects/golfBall", "ao.png", meta)};


        matComp.ShaderName = "pbr";
        auto* shader       = m_ShaderManager->GetShader(matComp.ShaderName);

        shader->ActivateShader();
        shader->SetValue("material.albedo", 0);
        shader->SetValue("material.normal", 1);
        shader->SetValue("material.ao", 2);

        shader->SetValue("light[0].position", Vec3{1.2f, 5.f, -2.f});
        shader->SetValue("light[0].color", Vec3{150.f, 150.f, 150.f});
        shader->SetValue("light[1].position", Vec3{5.2f, 0.3f, 2.f});
        shader->SetValue("light[1].color", Vec3{150.f, 150.f, 150.f});
        shader->SetValue("numLights", 2);

        shader->DeactivateShader();

        auto& transform      = entity->AddComponent<Transformation>();
        transform.Position.z = -1.f;
        transform.Position.y = -1.f;

        ID = entity->GetId();
        return m_EntityManager->LoadEntity(std::move(entity));
    }

    void Player::Tick() {}
} // namespace Game
