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

	string customPath = "/Heart.fbx";
	string ContentPath = FEnginePathHelper::GetEngineContentFBXPath();

	if (shared_ptr<ACustomMesh> CustomFBXmesh = World->CreateActor<ACustomMesh>("CustomFBXmesh"))
	{
		CustomFBXmesh->SetMeshComponent("CustomFBXmeshComponent", ContentPath + customPath, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
		if (auto CustomMaterial = make_shared<CMaterial>())
		{
			CustomMaterial->ResetGuid("CustomMaterial");//给创建的材质设置Guid
			CustomMaterial->SetMaterialType(EMaterialType::HalfLambert);
			CustomFBXmesh->SetSubMaterials(0, CustomMaterial);
		}
	}

}
