#include "OutlinerEditor.h"
#include "../Engine/Core/World.h"
#include "../Engine/Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#include "../Engine/Rendering/Core/DirectX/RenderingPipeline/RenderingLayer/RenderLayerManage.h"
#include "../Engine/Actor/Light/Core/Light.h"
#include "../Engine/Core/Camera.h"
#include "../Editor/SelectEditor/OperationHandle/Core/OperationHandleBase.h"


int FOutlinerEditor::selected = -1;

void FOutlinerEditor::BuildEditor()
{
	GetWorld()->GetCamera()->SelectedActorDelegate.AddFunction(this, &FOutlinerEditor::OnSelectedActor);
}

void FOutlinerEditor::DrawEditor(float DeltaTime)
{
	ImGui::Begin("Out Line");
	if (CWorld* world = GetWorld())
	{
		// 这里的大小写 00，可以自动适应文本大小
		ImGui::BeginChild("OutLine panel", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		const vector<AActor*>& WorldActors = world->GetWorldActors();

		for (int i = 0; i < WorldActors.size(); i++)
		{
			AActor* actor = WorldActors[i];
			char ObjectName[128] = { 0 };
			sprintf_s(ObjectName, "%s", actor->GetName());
			
			if (ObjectName == "") continue;

			//if (selected < 0)
			//{
			//	ImGui::Selectable(ObjectName, false);
			//	continue;
			//}

			if (ImGui::Selectable(ObjectName, selected == i))
			{
				selected = i;
				int index = -1;
				AMesh* mesh = dynamic_cast<AMesh*>(actor);
				
				if (mesh)
				{

					CMeshComponent* comp = mesh->GetMeshComponet<CMeshComponent>();
					if (comp && comp->GetIsPickup())
					{
						index = FGeometry::RenderingDataIndices[comp->GetGuid()];
					}
				}else
				{
					ALight* Light = dynamic_cast<ALight*>(actor);
					if (Light)
					{
						CMeshComponent* comp = Light->GetMeshComponet<CMeshComponent>();
						if (comp && comp->GetIsPickup())
						{
							index = FGeometry::RenderingDataIndices[comp->GetGuid()];
						}
					}
				}
				if (index > -1)
				{
					FRenderLayerManage::GetRenderLayerManage()->ClearGeometryDescData((int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT);
					FRenderLayerManage::GetRenderLayerManage()->AddGeometryDescData((int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT, FGeometry::MeshRenderingDataPool[index]);
					Engine_Log_Success("selectable selected = %d", i)
				}
				else
				{
					//Engine_Log_Error("selectable selected error", i)
				}
				
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void FOutlinerEditor::ExitEditor()
{
}

void FOutlinerEditor::OnSelectedActor(bool bSelected)
{
	if (CWorld* world = GetWorld())
	{
		if (bSelected)
		{
			if (!AOperationHandleBase::SelectedActor)
			{
				const vector<AActor*>& WorldActors = world->GetWorldActors();
				for (int i = 0; i < WorldActors.size(); i++)
				{
					const AActor* actor = WorldActors[i];
					if (AOperationHandleBase::SelectedActor->GetGuid() == actor->GetGuid())
					{
						selected = i;
					}
				}
			}
		}
		else
		{
			selected = -1;
		}
		
	}
}
