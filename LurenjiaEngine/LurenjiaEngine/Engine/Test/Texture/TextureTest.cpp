#include "TextureTest.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Mesh/PlaneMesh.h"

namespace TextureTest
{
	void TextureTest::BuildTextureTestData()
	{
		CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
		CWorld* World = WindowsEngine->GetRenderingEngine()->GetWorld();
		if (APlaneMesh* PlaneMesh = World->CreateActor<APlaneMesh>("PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("PlaneMeshComponent", 4.f, 3.f, 20, 20);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(30.f, 30.f, 1.f));
			if (CMaterial* PlaneMaterial = new CMaterial())
			{
				PlaneMaterial->ResetGuid("PlaneMateria");//给创建的材质设置Guid
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}
		if (ASphereMesh* SphereMesh_1 = World->CreateActor<ASphereMesh>("SphereMesh_1"))
		{
			SphereMesh_1->SetMeshComponent("SphereMesh_1Component", 2.f, 50, 50);
			SphereMesh_1->SetComponentPosition(XMFLOAT3(0.f, -5.f, 2.f));
			if (CMaterial* SphereMaterial_1 = new CMaterial())
			{
				SphereMaterial_1->ResetGuid("SphereMaterial_1");//给创建的材质设置Guid

				SphereMaterial_1->SetMaterialTextureMapKey("Wood2");
				SphereMaterial_1->SetSpecularColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
				SphereMaterial_1->SetRoughness(0.2f);
				SphereMaterial_1->SetMaterialType(EMaterialType::BlinnPhone);
				SphereMesh_1->SetSubMaterials(0, SphereMaterial_1);

			}
		}
		if (ASphereMesh* SphereMesh_2 = World->CreateActor<ASphereMesh>("SphereMesh_2"))
		{
			SphereMesh_2->SetMeshComponent("SphereMesh_2Component", 2.f, 50, 50);
			SphereMesh_2->SetComponentPosition(XMFLOAT3(0.f, 0.f, 2.f));
			if (CMaterial* SphereMaterial_2 = new CMaterial())
			{
				SphereMaterial_2->ResetGuid("SphereMaterial_2");//给创建的材质设置Guid
				SphereMaterial_2->SetMaterialTextureMapKey("Wood2");
				SphereMaterial_2->SetMaterialNormalMapKey("Wood2_Nor");
				SphereMaterial_2->SetSpecularColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
				SphereMaterial_2->SetRoughness(0.2f);
				SphereMaterial_2->SetMaterialType(EMaterialType::BlinnPhone);
				SphereMesh_2->SetSubMaterials(0, SphereMaterial_2);

			}
		}
		if (ASphereMesh* SphereMesh_3 = World->CreateActor<ASphereMesh>("SphereMesh_3"))
		{
			SphereMesh_3->SetMeshComponent("SphereMesh_32Component", 2.f, 50, 50);
			SphereMesh_3->SetComponentPosition(XMFLOAT3(0.f, 5.f, 2.f));
			if (CMaterial* SphereMaterial_3 = new CMaterial())
			{
				SphereMaterial_3->ResetGuid("SphereMaterial_3");//给创建的材质设置Guid
				SphereMaterial_3->SetMaterialTextureMapKey("Wood2");
				SphereMaterial_3->SetMaterialNormalMapKey("Wood2_Nor");
				SphereMaterial_3->SetMaterialSpecularMapKey("Wood2_SPEC");
				SphereMaterial_3->SetRoughness(0.2f);
				SphereMaterial_3->SetMaterialType(EMaterialType::BlinnPhone);
				SphereMesh_3->SetSubMaterials(0, SphereMaterial_3);

			}
		}
	}
}

