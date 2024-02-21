#include "LogEditor.h"
#include "Core/LogSystem.h"

void FLogEditor::BuildEditor()
{
}

void FLogEditor::DrawEditor(float DeltaTime)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	bool bOpenLog = true;
	ImGui::Begin("Log", &bOpenLog);
	//ImGui::Text("gaozhanpeng test");
	ImGui::End();


	FEditorLogSystem::GetEditorLogSystem()->Draw(DeltaTime);
}

void FLogEditor::ExitEditor()
{
	FEditorLogSystem::Destory();
}
