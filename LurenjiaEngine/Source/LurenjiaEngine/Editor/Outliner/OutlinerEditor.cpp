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
	if (shared_ptr<CWorld> world = GetWorld())
	{
		// 这里的大小写 00，可以自动适应文本大小
		ImGui::BeginChild("OutLine panel", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		const vector<shared_ptr<AActor>>& WorldActors = world->GetWorldActors();

		for (int i = 0; i < WorldActors.size(); i++)
		{
			const shared_ptr<AActor>& actor = WorldActors[i];
			char ObjectName[128] = { 0 };
			sprintf_s(ObjectName, "%s", actor->GetName().c_str());

			if (selected < 0)
			{
				ImGui::Selectable(ObjectName, false);
				continue;
			}

			if (ImGui::Selectable(ObjectName, selected == i))
			{
				selected = i;
				int index = -1;
				const shared_ptr<AMesh>& mesh = dynamic_pointer_cast<AMesh>(actor);
				
				if (mesh)
				{

					shared_ptr<CMeshComponent> comp = mesh->GetMeshComponet<CMeshComponent>();
					if (comp && comp->GetIsPickup())
					{
						index = FGeometry::RenderingDataIndices[comp->GetGuid()];
					}
				}else
				{
					const shared_ptr<ALight>& Light = dynamic_pointer_cast<ALight>(actor);
					if (Light)
					{
						shared_ptr<CMeshComponent> comp = Light->GetMeshComponet<CMeshComponent>();
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
	if (shared_ptr<CWorld> world = GetWorld())
	{
		if (bSelected)
		{
			if (!AOperationHandleBase::SelectedActor.expired())
			{
				const vector<shared_ptr<AActor>>& WorldActors = world->GetWorldActors();
				for (int i = 0; i < WorldActors.size(); i++)
				{
					const shared_ptr<AActor>& actor = WorldActors[i];
					if (AOperationHandleBase::SelectedActor.lock()->GetGuid() == actor->GetGuid())
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
