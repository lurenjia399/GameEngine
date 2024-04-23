#include "LoadFBXModelTest.h"
#include "../../Actor/Mesh/Core/Material/Material.h"
#include "../../Actor/Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Actor/Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Actor/Mesh/BoxMesh.h"
#include "../../Actor/Mesh/CustomMesh.h"

void LoadFBXModelTest::BuildLoadFBXModelTestData()
{
	shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
	shared_ptr<CWorld> World = WindowsEngine->GetRenderingEngine()->GetWorld();

	string customPath = "/SK_Mannequin.fbx";
	//string customPath = "/56-fbx/Audi/Models/Audi_R8.fbx";
	string ContentPath = FEnginePathHelper::GetEngineContentFBXPath();

	if (shared_ptr<ACustomMesh> CustomFBXmesh = World->CreateActor<ACustomMesh>("CustomFBXmesh"))
	{
		CustomFBXmesh->SetMeshComponent("CustomFBXmeshComponent", ContentPath + customPath, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
		//CustomFBXmesh->SetPosition(XMFLOAT3(0, 0, 0));
		CustomFBXmesh->SetComponentPosition(XMFLOAT3(0, 0, 0));
		if (auto CustomMaterial = make_shared<CMaterial>())
		{
			CustomMaterial->ResetGuid("CustomMaterial");//给创建的材质设置Guid
			CustomMaterial->SetMaterialType(EMaterialType::HalfLambert);
			CustomFBXmesh->SetSubMaterials(0, CustomMaterial);
		}
	}

	// 平行光
	if (auto ParallelLight = World->CreateActor<AParallelLight>("SelectActorTest::AParallelLight"))
	{
		ParallelLight->SetLightIntensity(XMFLOAT3(1.0f, 1.0f, 1.0f));
		ParallelLight->SetPosition(XMFLOAT3(-20.f, 0.f, 40.f));
		ParallelLight->SetRotateFunction([=](float time) ->void
			{
				float angle_speed = (float)PI;
				float angle = time * angle_speed;
				XMFLOAT3 cur_pos = ParallelLight->GetPosition();
				XMFLOAT3 ret = cur_pos;
				float s = sin(angle);
				float c = cos(angle);
				ret = XMFLOAT3(cur_pos.x * c - cur_pos.y * s, cur_pos.x * s + cur_pos.y * c, cur_pos.z);
				ParallelLight->SetPosition(ret);
				ParallelLight->FaceTarget(ret, XMFLOAT3(0, 0.f, 40.f), XMFLOAT3(0.f, 0.f, 1.f));
			});
	}

}
