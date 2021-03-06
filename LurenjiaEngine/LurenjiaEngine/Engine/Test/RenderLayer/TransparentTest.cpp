#include "TransparentTest.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Component/Mesh/SphereMeshComponent.h"

namespace TransparentTest
{
	void BuildTransparentTestData()
	{
		CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
		CWorld* World = WindowsEngine->GetRenderingEngine()->GetWorld();
		if (ASphereMesh* SphereMesh = World->CreateActor<ASphereMesh>("SphereMesh"))
		{
			
			SphereMesh->SetMeshComponent("SphereMeshComponent", 2.f, 50, 50, EMeshComponentRenderLayerType::RENDERLAYER_TRANSPARENT);
			SphereMesh->SetComponentPosition(XMFLOAT3(0.f, -5.f, 7.f));
			if (CMaterial* SphereMaterial = new CMaterial())
			{
				SphereMaterial->ResetGuid("SphereMaterial");//给创建的材质设置Guid
				SphereMaterial->SetMaterialTextureMapKey("TransparentContent");
				SphereMaterial->SetMaterialType(EMaterialType::HalfLambert);
				SphereMesh->SetSubMaterials(0, SphereMaterial);

			}
		}
	}
}