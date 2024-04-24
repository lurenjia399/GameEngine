#include "EditorEngine.h"
#include "ToolbarEditor/ToolbarEditor.h"
#include "LogEditor/LogEditor.h"
#include "Outliner/OutlinerEditor.h"
#include "SelectEditor/SelectEditor.h"

CEditorEngine::CEditorEngine()
    : ToolbarEditor(new FToolbarEditor())
    , LogEditor(new FLogEditor())
    , OutlinerEditor(new FOutlinerEditor())
    , SelectEditor(new FSelectEditor())
{
    // 注意这个地方，new了变量，没地方把他销毁
    
    //ToolbarEditor->ResetGuid("Editor::ToolbarEditor");
    //LogEditor->ResetGuid("Editor::LogEditor");
}

int CEditorEngine::PreInit(FWinMainCommandParameters& InParameters)
{
    return 0;
}

int CEditorEngine::Init(FWinMainCommandParameters InParameters)
{
    return 0;
}

int CEditorEngine::PostInit()
{
    return 0;
}

void CEditorEngine::Tick(float DeltaTime)
{
}

int CEditorEngine::PreExit()
{
    return 0;
}

int CEditorEngine::Exit()
{
    return 0;
}

int CEditorEngine::PostExit()
{
    return 0;
}

void CEditorEngine::BuildEditor()
{
    // 开启吸附功能
    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ToolbarEditor->BuildEditor();
    LogEditor->BuildEditor();
    OutlinerEditor->BuildEditor();
    SelectEditor->BuildEditor();
}

void CEditorEngine::DrawEditor(float DeltaTime)
{

    //bool show_demo_window = true;
    //ImGui::ShowDemoWindow(&show_demo_window); // 官方提供的学习案例

    DrawLayer(DeltaTime); // 主要是弄全屏吸附的部分

    ToolbarEditor->DrawEditor(DeltaTime);
    LogEditor->DrawEditor(DeltaTime);
    OutlinerEditor->DrawEditor(DeltaTime);
    SelectEditor->DrawEditor(DeltaTime);
}

void CEditorEngine::ExitEditor()
{
    ToolbarEditor->ExitEditor();
    LogEditor->ExitEditor();
    OutlinerEditor->ExitEditor();
    SelectEditor->ExitEditor();
}

void CEditorEngine::DrawLayer(float DeltaTime)
{
    ImGuiDockNodeFlags DockNodeFlags = ImGuiDockNodeFlags_::ImGuiDockNodeFlags_None;
    // 吸附类的，懒得管是啥了
    DockNodeFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

    const ImGuiViewport* Viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(Viewport->WorkPos);
    ImGui::SetNextWindowSize(Viewport->WorkSize);
    ImGui::SetNextWindowViewport(Viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // 禁用吸附
    ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDocking;
    // 禁用标题bar，禁用折叠就是最小化，禁用缩放，禁用移动
    WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    // 禁用聚焦也就是一直在最下面点不到，禁用导航聚焦也就是键盘无法聚焦到
    WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    // 禁用掉背景颜色
    WindowFlags |= ImGuiWindowFlags_NoBackground;

    bool bOpenWindows = true;
    ImGui::Begin("LurenjiaEngine", &bOpenWindows, WindowFlags);
    {
        ImGui::PopStyleVar(3);
        ImGuiIO& IO = ImGui::GetIO();
        if (IO.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID DockSpaceID = ImGui::GetID("Lurenjia_DockSpaceID");
            ImGui::DockSpace(DockSpaceID, ImVec2(0.f, 0.f), DockNodeFlags);
        }
    }
    ImGui::End();
}
