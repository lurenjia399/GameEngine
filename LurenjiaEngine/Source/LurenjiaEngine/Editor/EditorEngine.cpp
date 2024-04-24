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
    // ע������ط���new�˱�����û�ط���������
    
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
    // ������������
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
    //ImGui::ShowDemoWindow(&show_demo_window); // �ٷ��ṩ��ѧϰ����

    DrawLayer(DeltaTime); // ��Ҫ��Ūȫ�������Ĳ���

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
    // ������ģ����ù���ɶ��
    DockNodeFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

    const ImGuiViewport* Viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(Viewport->WorkPos);
    ImGui::SetNextWindowSize(Viewport->WorkSize);
    ImGui::SetNextWindowViewport(Viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // ��������
    ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDocking;
    // ���ñ���bar�������۵�������С�����������ţ������ƶ�
    WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    // ���þ۽�Ҳ����һֱ��������㲻�������õ����۽�Ҳ���Ǽ����޷��۽���
    WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    // ���õ�������ɫ
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
