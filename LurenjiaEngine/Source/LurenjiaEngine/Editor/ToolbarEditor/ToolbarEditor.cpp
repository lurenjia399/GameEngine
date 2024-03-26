#include "ToolbarEditor.h"

void FToolbarEditor::BuildEditor()
{
}

void FToolbarEditor::DrawEditor(float DeltaTime)
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

		ImGui::Separator(); // ·Ö¸ô·û

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void FToolbarEditor::ExitEditor()
{
}
