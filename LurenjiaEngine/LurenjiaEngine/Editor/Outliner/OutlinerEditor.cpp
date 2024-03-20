#include "OutlinerEditor.h"
#include "../Engine/Core/World.h"
#include "../Engine/Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#include "../Engine/Rendering/Core/DirectX/RenderingPipeline/RenderingLayer/RenderLayerManage.h"

void FOutlinerEditor::BuildEditor()
{
}

void FOutlinerEditor::DrawEditor(float DeltaTime)
{
	ImGui::Begin("Out Line");
	if (shared_ptr<CWorld> world = GetWorld())
	{
		// ����Ĵ�Сд 00�������Զ���Ӧ�ı���С
		ImGui::BeginChild("OutLine panel", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		const vector<shared_ptr<AActor>>& WorldActors = world->GetWorldActors();

		static int selected = 0;
		for (int i = 0; i < WorldActors.size(); i++)
		{
			const shared_ptr<AActor>& actor = WorldActors[i];
			char ObjectName[128] = { 0 };
			sprintf(ObjectName, "%s", actor->GetName().c_str());
			if (ImGui::Selectable(ObjectName, selected == i))
			{
				selected = i;
				const shared_ptr<AMesh>& mesh = static_pointer_cast<AMesh>(actor);
				// ����ѡ�еƹ��ʱ��������ԭ���ǵƹ�actor��û��meshcomponent
				const shared_ptr<CMeshComponent>& comp = mesh->GetMeshComponet<CMeshComponent>();
				int index = FGeometry::RenderingDataIndices[comp->GetGuid()];
				
				FRenderLayerManage::GetRenderLayerManage()->ClearGeometryDescData((int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT);
				FRenderLayerManage::GetRenderLayerManage()->AddGeometryDescData((int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT, FGeometry::MeshRenderingDataPool[index]);
				Engine_Log_Success("selectable selected = %d", i)
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void FOutlinerEditor::ExitEditor()
{
}
