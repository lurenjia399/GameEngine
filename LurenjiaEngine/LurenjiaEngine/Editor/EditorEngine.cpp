#include "EditorEngine.h"
#include "ToolbarEditor/ToolbarEditor.h"
#include "LogEditor/LogEditor.h"

CEditorEngine::CEditorEngine()
    : ToolbarEditor(new FToolbarEditor())
    , LogEditor(new FLogEditor())
{
    // 注意这个地方，new了两个变量，没地方把他销毁
    
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
    ToolbarEditor->BuildEditor();
    LogEditor->BuildEditor();
}

void CEditorEngine::DrawEditor(float DeltaTime)
{

    ToolbarEditor->DrawEditor(DeltaTime);
    LogEditor->DrawEditor(DeltaTime);
}

void CEditorEngine::ExitEditor()
{
    ToolbarEditor->ExitEditor();
    LogEditor->ExitEditor();
}