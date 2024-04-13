#include "LoadFBXModelTest.h"
#include "../../Actor/Mesh/Core/Material/Material.h"
#include "../../Actor/Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Actor/Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Actor/Mesh/BoxMesh.h"

void LoadFBXModelTest::BuildLoadFBXModelTestData()
{
	shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
	shared_ptr<CWorld> World = WindowsEngine->GetRenderingEngine()->GetWorld();

	string ParalLelLightPath = "/SunMesh.obj";
	string ContentPath = FEnginePathHelper::GetEngineContentPath();
	//LightMeshComponent = GetMeshManage()->CreateCustomMeshComponent("ParallelLightMeshComponent", ContentPath + ParalLelLightPath);

	//if (LightMeshComponent)
	//{
		//shared_ptr<CMaterial> material = LightMeshComponent->GetMaterials()[0];
		//material->SetMaterialType(EMaterialType::BaseColor);
		//material->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
		//material->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	//}

}
