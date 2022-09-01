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

/*
inline String GetLightType(LightType type)
{
    switch(type)
    {
    case LightType::DIRECTIONAL: return "Directional";
    case LightType::POINT: return "Point";
    case LightType::SPOT: return "Spot";
    case LightType::DISABLED: return "Disabled";
    default: return "";
    }
}
*/
inline static int idx = 0;
static int size;
/*
void Window::Gui::m_DrawLightMenu() const
{
    ImGui::Text("Light");

    ImGui::Indent(10.f);
    ImGui::BeginGroup();

    size = LIGHT.ms_PLights.size();
    ImGui::SliderInt("", &(++idx), 1, size + 1, (idx < size ? "%d" : "Flashlight"));
    if(--idx == size)
    {
        ImGui::SameLine();
        ImGui::Checkbox("Flashlight", &LIGHT.Flashlight);
    }

    ImGui::InputFloat3("Position", &LIGHT[idx].Position[0]);
    ImGui::InputFloat3("Ambient", &LIGHT[idx].Ambient[0]);
    ImGui::InputFloat3("Diffuse", &LIGHT[idx].Diffuse[0]);
    ImGui::InputFloat3("Specular", &LIGHT[idx].Specular[0]);
    ImGui::InputFloat3("Color", &LIGHT[idx].Color[0]);

    if(LIGHT[idx].Type == LightType::POINT || LIGHT[idx].Type == LightType::SPOT)
    {
        ImGui::SliderFloat2("Linear - Quadratic", &LIGHT[idx].linear, 0.001f, 1.7f);

        if(LIGHT[idx].Type == LightType::SPOT)
        {
            ImGui::InputFloat3("Direction", &LIGHT[idx].Direction[0]);
            ImGui::SliderFloat("Angle", &LIGHT[idx].CutoffAngle, 0.f, 90.f);
        }
    }


    if(idx != size && ImGui::BeginMenu(GetLightType(LIGHT[idx].Type).c_str()))
    {
        if(ImGui::MenuItem("Directional"))
        {
            LIGHT[idx].Type = LightType::DIRECTIONAL;
        }
        if(ImGui::MenuItem("Point"))
        {
            LIGHT[idx].Type = LightType::POINT;
        }
        if(ImGui::MenuItem("Spot"))
        {
            LIGHT[idx].Type = LightType::SPOT;
        }
        if(ImGui::MenuItem("Disabled"))
        {
            LIGHT[idx].Type = LightType::DISABLED;
        }
        ImGui::EndMenu();
    }


    if(ImGui::Button("Add light") && size < 9)
    {
        LIGHT.ms_PLights.emplace_back(PhongLight{});
        idx = size++;
    }

    if(size == 9)
    {
        ImGui::SameLine();
        ImGui::TextColored({1.f, 0.f, 0.f, 1.f}, "Max number of lights added");
    }

    ImGui::EndGroup();
    ImGui::Indent(-10.f);
    ImGui::Separator();
}
*/

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
    /*
    ImGui::Text("Camera instance\n Transformation:");
    ImGui::InputFloat3("Position", &comp->Position[0]);
    ImGui::InputFloat3("Rotation", &comp->Rotation.Pitch);
    ImGui::InputFloat3("Scale", &comp->Scale[0]);
    */
}

// --------------------------------------------------------------------------------------------

/*
void Window::Gui::m_DrawCameraMenu()
{
    TVector<Entities::Entity*> entities =
        m_EntityManager->GetEntitiesWithComponents<Component::Camera, Component::Transformation>();
    ImGui::Text("Camera");

    ImGui::Indent(10.f);


    for(auto* e : entities)
    {
        ImGui::BeginGroup();
        auto* comp = e->GetComponent<Component::Transformation>();
        DrawTransform(comp);

        // auto* camera = e->GetComponent<Component::Camera>();
        
        // ImGui::Text("Position: (%.3f, %.3f, %.3f)", pos.x, pos.y, pos.z);
        // auto& pos = CAMERA.GetCameraPos();
        // ImGui::SliderFloat("Movement speed", &CAMERA.MovementSpeed, 0.f, 50.f);
        // ImGui::SliderFloat("Sensitivity", &CAMERA.MouseSensitivity, 0.f, 1.f);
        

        ImGui::EndGroup();
    }
    ImGui::Indent(-10.f);
    ImGui::Separator();
}
*/
// --------------------------------------------------------------------------------------------

