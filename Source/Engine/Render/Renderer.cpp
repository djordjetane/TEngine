#include <precomp.h>
#include <Window/Window.h>
#include <Window/Gui.h>
#include <Shader/ShaderManager.h>

#include "Camera/Camera.h"
#include "Components/Components.h"
#include "PostProcessing/Framebuffer.h"
#include "Vertex/Buffer.h"
#include "Vertex/VertexArray.h"
#include "Renderer.h"


bool Render::Renderer::Init()
{
    m_Window = new Window::Window;
    SMASSERT(m_Window->Init(), "FAILED TO INIT MAIN WINDOW");

    SMASSERT(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Glad failed to load gl");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);

    glViewport(0, 0, WND_WIDTH, WND_HEIGHT);

    glfwSetFramebufferSizeCallback(m_Window->GetNativeWindow(), Window::Window::FrameBufferResizeCallback);

    m_ShaderManager = new ShaderManager;
    SMASSERT(m_ShaderManager->m_LoadDefaultShaders(), "Failed to load default shaders!");

    m_FrameBuff = new Framebuffer();
    SMASSERT(m_FrameBuff->Init(m_ShaderManager->GetShader("screen")), "Failed to initialize framebuffer");

    SMASSERT(m_Gui->Init(m_Window->GetNativeWindow()), "Failed to initialize Debug Menu");

    return true;
}

bool Render::Renderer::IsRunning() const { return m_Window->IsRunning(); }

void Render::Renderer::StartScene() const
{
    m_FrameBuff->BindSceneBegin();
    m_ClearColor();
}

void Render::Renderer::EndScene() const
{
    m_FrameBuff->BindSceneEnd();
    m_Gui->Update();
    m_Window->Update();
}


void Render::Renderer::DrawEntities(std::vector<Entities::Entity*>& entities, Entities::Entity* camera) const
{
    auto *cameraComp  = camera->GetComponent<Component::Camera>();
    auto *cameraTrans = camera->GetComponent<Component::Transformation>();

    Mat4  PV          = cameraComp->GetProjection() * Math::ViewMatrix(cameraTrans->Position, cameraTrans->Rotation);

    for(auto* e : entities)
    {
        if(e->HasComponent<Component::Material>())
            DrawMaterial(e, camera, PV);
        else
            DrawSprite(e, camera);
    }
}

void Render::Renderer::DrawMaterial(Entities::Entity* entity, Entities::Entity* camera, const Mat4& PV) const
{
    auto* cameraTrans = camera->GetComponent<Component::Transformation>();
    //auto* cameraComp  = camera->GetComponent<Component::Camera>();


    auto* material = entity->GetComponent<Component::Material>();
    auto* mesh     = entity->GetComponent<Component::Mesh>();
    if(material == nullptr || mesh == nullptr)
        return;
    auto* shader = m_ShaderManager->GetShader(material->ShaderName);
    auto* transf = entity->GetComponent<Component::Transformation>();

    shader->ActivateShader();
    
    shader->SetValue("PV", PV);
    shader->SetValue("model", transf->GetModelMatrix());
    shader->SetValue("viewPos", cameraTrans->Position);

    mesh->VArray.Bind();

    for(size_t i = 0; i < material->Textures.size(); i++)
        material->Textures[i]->Activate(i);
    mesh->VArray.DrawInstanced(100);
    //mesh->VArray.Draw();

    for(size_t i = material->Textures.size(); i > 0; i--)
        material->Textures[i - 1]->Deactivate(i - 1);
    mesh->VArray.Unbind();

    shader->DeactivateShader();
}


void Render::Renderer::DrawSprite(Entities::Entity* entity, Entities::Entity* camera) const
{
    auto* sprite    = entity->GetComponent<Component::Sprite>();
    auto* transform = entity->GetComponent<Component::Transformation2D>();
    auto* mesh      = entity->GetComponent<Component::Mesh>();
    if(!(sprite && transform))
        return;

    static auto* shader = m_ShaderManager->GetShader("sprite");
    shader->ActivateShader();
    auto* cameraComp = entity->GetComponent<Component::Camera>();
    shader->SetValue("PM", cameraComp->GetProjection() * transform->GetModelMatrix());
    shader->SetValue("spriteColor", sprite->Color);
    mesh->VArray.Bind();

    sprite->Texture->Activate(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    mesh->VArray.Unbind();
    shader->DeactivateShader();
}

void Render::Renderer::DrawCubeMap(Entities::Entity* entity, Entities::Entity* camera) const
{
    glDepthFunc(GL_LEQUAL);
    auto* cameraComp = camera->GetComponent<Component::Camera>();
    auto* transComp  = camera->GetComponent<Component::Transformation>();

    auto* comp = entity->GetComponent<Component::Sky>();


    comp->Shader->ActivateShader();
    comp->Shader->SetValue("PV",
                          cameraComp->GetProjection() * Math::CubemapMatrix(transComp->Position, transComp->Rotation));
    comp->VArray.Bind();
    comp->Texture->Activate();
    comp->VArray.Draw();

    comp->Texture->Deactivate();
    comp->VArray.Unbind();
    glDepthFunc(GL_LESS);
    comp->Shader->DeactivateShader();
}

void Render::Renderer::m_ClearColor() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

Render::Renderer::~Renderer()
{
    delete m_FrameBuff;
#ifdef _DEBUG
    m_Gui->Destroy();
    delete m_Gui;
#endif
    m_Window->Destroy();
    delete m_Window;
    delete m_ShaderManager;
    glfwTerminate();
}
