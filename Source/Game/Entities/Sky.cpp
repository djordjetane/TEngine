#include "Sky.h"
#include "GameData.h"

namespace Game {
    bool Sky::Init(Engine::EntityManager* em, Engine::ShaderManager* sm, Engine::TextureManager* tm)
    {
        m_EntityManager  = em;
        m_ShaderManager  = sm;
        m_TextureManager = tm;
        
        auto* entity = em->NewEntity();
        auto& comp = entity->AddComponent<Component::Sky>();
        
        comp.Texture = tm->LoadCubemap(CubeMapFaces);
        SASSERT(comp.Texture != nullptr, "Failed to load skybox texture");

        comp.Shader  = sm->GetShader("skybox");
        SASSERT(comp.Texture != nullptr, "Failed to load skybox texture");

        comp.Shader->ActivateShader();
        comp.Shader->SetValue("skybox", 0);
        comp.Shader->DeactivateShader();

        return true;
    }

    void Sky::Tick()
    {
        
    }
}
