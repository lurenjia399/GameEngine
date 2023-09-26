#include "ShadowMapTest.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Mesh/BoxMesh.h"

namespace ShadowMapTest
{
	void ShadowMapTest::BuildShadowMapTestData()
	{
		CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
		CWorld* World = WindowsEngine->GetRenderingEngine()->GetWorld();

		// cubeMap
		if (ABoxMesh* CubeMesh = World->CreateActor<ABoxMesh>("ABoxMesh"))
		{
			CubeMesh->SetMeshComponent("ABoxMeshComponent", 10, 10, 10, EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND);
			CubeMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, 0.f));
			CubeMesh->SetComponentScale(XMFLOAT3(100.f, 100.f, 100.f));
			if (CMaterial* CubeMaterial = new CMaterial())
			{
				CubeMaterial->ResetGuid("CubeMaterial");//给创建的材质设置Guid
				CubeMaterial->SetMaterialTextureMapKey("x1_CubeMap");
				CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
				CubeMesh->SetSubMaterials(0, CubeMaterial);
			}
		}

		//平行光
		if (AParallelLight* ParallelLight = World->CreateActor<AParallelLight>("AParallelLight"))
		{
			ParallelLight->SetLightIntensity(XMFLOAT3(1.0f, 1.0f, 1.0f));
			ParallelLight->SetPosition(XMFLOAT3(-30.f, 0.f, 70.f));
			//ParallelLight->SetRotation(fvector_3d(0.f, 0.f, -90.0f));
		}


		if (APlaneMesh* PlaneMesh = World->CreateActor<APlaneMesh>("ShadowMapTest::PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("ShadowMapTest::PlaneMeshComponent", 4.f, 3.f, 20, 20, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(30.f, 30.f, 1.f));
			if (CMaterial* PlaneMaterial = new CMaterial())
			{
				PlaneMaterial->ResetGuid("ShadowMapTest::PlaneMateria");
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}
		for (int height = 0; height < 2; ++height)
		{
			for (int row = 0; row < 3; ++row)
			{
				for (int col = 0; col < 3; ++col)
				{
					// 球的位置
					XMFLOAT3 Position(height * 7.f, col * 7.f - 7.0f, row * (-5.f) + 11.0f);

					int index = height * 100 + row * 10 + col;
					if (ASphereMesh* SphereMesh = World->CreateActor<ASphereMesh>("ShadowMapTest::SphereMesh_" + index))
					{
						SphereMesh->SetMeshComponent("ShadowMapTest::SphereMeshComponent_" + index, 2.f, 50, 50);
						SphereMesh->SetComponentPosition(Position);
						if (CMaterial* SphereMaterial = new CMaterial())
						{
							SphereMaterial->ResetGuid("ShadowMapTest::SphereMaterial_" + index);
							//SphereMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.0f));
							SphereMaterial->SetMaterialType(EMaterialType::Lambert);
							SphereMesh->SetSubMaterials(0, SphereMaterial);
						}
					}

				}
			}
		}
		
		if (ABoxMesh* BoxMesh = World->CreateActor<ABoxMesh>("ShadowMapTest::BoxMesh"))
		{
			BoxMesh->SetMeshComponent("ShadowMapTest::BoxMeshComponent", 3, 3, 3);
			BoxMesh->SetComponentPosition(XMFLOAT3(0.f, 12.f, 1.f));
			if (CMaterial* BoxMeshMaterial = new CMaterial())
			{
				BoxMeshMaterial->ResetGuid("ShadowMapTest::BoxMeshMaterial");//给创建的材质设置Guid
				BoxMeshMaterial->SetMaterialType(EMaterialType::Lambert);
				BoxMesh->SetSubMaterials(0, BoxMeshMaterial);
			}
		}
	}
}