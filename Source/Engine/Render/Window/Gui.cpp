#include <precomp.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Components/Components.h"
#include "Gui.h"


/* Initialize after setting up native opengl viewport */
bool Window::Gui::Init(NativeWindow* window, EntMan* entityManager)
{
    nativeWindow    = window;
    m_EntityManager = m_EntityManager ? m_EntityManager : entityManager;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= m_ViewportsEnable ? ImGuiConfigFlags_ViewportsEnable : 0;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    bool retval = ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);
    return retval && ImGui_ImplOpenGL3_Init(WindowData::glslVersion);
}

bool Window::Gui::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool pOpen = true;

    ImGui::Begin("GuiWindow", &pOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);

    mDrawGFXSettings();
    //-------------------------------
    if(m_EntityManager != nullptr)
    {
        mDrawEntityList();
    }
    //-------------------------------

    float frametime = 1000.f / ImGui::GetIO().Framerate;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", frametime, ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_ViewportsEnable = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0;

    if(m_ViewportsEnable)
    {
        NativeWindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }

    return true;
}

void Window::Gui::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    m_GuiActive = false;
}

inline static int idx = 0;
static int size;

void Window::Gui::mDrawGFXSettings() const
{
    ImGui::Text("Graphics settings");

    ImGui::Indent(10.f);
    ImGui::BeginGroup();

    if(ImGui::BeginMenu("Resolution"))
    {
        if(ImGui::MenuItem("1280x720"))
        {
            glfwSetWindowSize(nativeWindow, 1280, 720);
            glViewport(0, 0, 1280, 720);
        }
        if(ImGui::MenuItem("1920x1080"))
        {
            glfwSetWindowSize(nativeWindow, 1920, 1080);
            glViewport(0, 0, 1920, 1080);
        }

        ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Anti-Aliasing"))
    {
        if(ImGui::MenuItem("None"))
        {
            WindowData::fxaa = false;
        }
        if(ImGui::MenuItem("FXAA"))
        {
            WindowData::fxaa = true;
        }

        ImGui::EndMenu();
    }

    ImGui::EndGroup();

    if(ImGui::Checkbox("VSync", &WindowData::vsync))
    {
    }
    ImGui::Indent(-10.f);
    ImGui::Separator();
}

void Window::Gui::mDrawEntityList() const
{
    ImGui::Text("Scene:");

    ImGui::Indent(10.f);
    ImGui::BeginGroup();

    for(auto& entity : m_EntityManager->m_Table)
    {
        String label{"Entity "};
        label.append(std::to_string(entity->GetId()));
        ImGui::Button(label.c_str());
    }

    ImGui::EndGroup();
}

// ---------------------------------------------------------------------------

inline void DrawTransform(Component::Transformation* comp)
{
    
}

// --------------------------------------------------------------------------------------------
