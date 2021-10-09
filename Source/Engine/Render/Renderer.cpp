#include <precomp.h>
#include <Window/Window.h>
#ifdef _DEBUG
#include <Window/DebugWindow.h>
#endif
#include <Shader/ShaderManager.h>

#include "Camera/Camera.h"
#include "Components/Components.h"
#include "PostProcessing/Framebuffer.h"
#include "Renderer.h"
#include "Vertex/Buffer.h"
#include "Vertex/VertexArray.h"


bool Render::Renderer::Init()
{
    m_Window = new Window::Window;
    SMASSERT(m_Window->Init(), "FAILED TO INIT MAIN WINDOW");

    // SMASSERT(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Glad failed to load gl");
    SMASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Glad failed to load gl");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);

    glViewport(0, 0, WND_WIDTH, WND_HEIGHT);

    glfwSetFramebufferSizeCallback(m_Window->GetNativeWindow(), Window::Window::FrameBufferResizeCallback);

    m_ShaderManager = new ShaderManager;
    // TODO: Replace this vector
    const std::vector<String> shaders{"sprite", "pbr", "screen", "terrain", "skybox"};
    for(auto s : shaders)
    {
        SMAASSERT(m_ShaderManager->AddShader(s), "Failed to add shader: {}", s);
    }

    m_FrameBuff = new Framebuffer();
    SMASSERT(m_FrameBuff->Init(m_ShaderManager->GetShader("screen")), "Failed to initialize framebuffer");

#ifdef _DEBUG
    // m_DebugWindow = new Window::DebugWindow;
    SMASSERT(m_DebugWindow->Init(m_Window->GetNativeWindow()), "Failed to initialize Debug Menu");
#endif

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
#ifdef _DEBUG
    m_DebugWindow->Update();
#endif
    m_Window->Update();
}


void Render::Renderer::DrawEntities(std::vector<Entities::Entity*>& entities, Entities::Entity* camera) const
{
    for(auto* e : entities)
    {
        if(e->HasComponent<Component::Material>())
            DrawMaterial(e, camera);
        else
            DrawSprite(e, camera);
    }
}

void Render::Renderer::DrawMaterial(Entities::Entity* entity, Entities::Entity* camera) const
{
    auto* cameraTrans = camera->GetComponent<Component::Transformation>();
    auto* cameraComp  = camera->GetComponent<Component::Camera>();


    auto* material = entity->GetComponent<Component::Material>();
    auto* mesh     = entity->GetComponent<Component::Mesh>();
    if(material == nullptr || mesh == nullptr)
        return;
    auto* shader = m_ShaderManager->GetShader(material->ShaderName);
    auto* transf = entity->GetComponent<Component::Transformation>();

    shader->ActivateShader();
    shader->SetMat4("PV", cameraComp->GetProjection() * Math::ViewMatrix(cameraTrans->Position, cameraTrans->Rotation));
    shader->SetMat4("model", transf->GetModelMatrix());
    shader->SetVec3("viewPos", cameraTrans->Position);

    mesh->VArray.Bind();

    for(size_t i = 0; i < material->Textures.size(); i++)
        material->Textures[i]->Activate(i);
    mesh->VArray.Draw();

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
    shader->SetMat4("PM", cameraComp->GetProjection() * transform->GetModelMatrix());
    shader->SetVec3("spriteColor", sprite->Color);
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
    comp->Shader->SetMat4("PV",
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
    m_DebugWindow->Destroy();
    delete m_DebugWindow;
#endif
    m_Window->Destroy();
    delete m_Window;
    delete m_ShaderManager;
    glfwTerminate();
}
