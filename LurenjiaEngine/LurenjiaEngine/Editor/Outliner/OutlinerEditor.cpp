#include "OutlinerEditor.h"

void FOutlinerEditor::BuildEditor()
{
}

void FOutlinerEditor::DrawEditor(float DeltaTime)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Level"))
			{

			}
			if (ImGui::MenuItem("Open Level"))
			{

			}
			if (ImGui::MenuItem("Save Asset"))
			{

			}


			ImGui::EndMenu();
		}

		ImGui::Separator(); // �ָ���

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void FOutlinerEditor::ExitEditor()
{
}
