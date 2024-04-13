#include "ShadowMapTest.h"
#include "../../Actor/Mesh/Core/Material/Material.h"
#include "../../Actor/Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Actor/Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Actor/Mesh/BoxMesh.h"

#define SHOWLIGHTMESH 1

namespace ShadowMapTest
{
	void ShadowMapTest::BuildShadowMapTestData()
	{
		shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
		shared_ptr<CWorld> World = WindowsEngine->GetRenderingEngine()->GetWorld();

		// cubeMap
		if (auto CubeMesh = World->CreateActor<ABoxMesh>("ABoxMesh"))
		{
			CubeMesh->SetMeshComponent("ABoxMeshComponent", 10, 10, 10, EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND);
			CubeMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, 0.f));
			CubeMesh->SetComponentScale(XMFLOAT3(100.f, 100.f, 100.f));
			if (auto CubeMaterial = make_shared<CMaterial>())
			{
				CubeMaterial->ResetGuid("CubeMaterial");//给创建的材质设置Guid
				CubeMaterial->SetMaterialTextureMapKey("x1_CubeMap");
				CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
				CubeMesh->SetSubMaterials(0, CubeMaterial);
			}
		}


		//平行光
		if (auto ParallelLight = World->CreateActor<AParallelLight>("AParallelLight"))
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
				//ParallelLight->SetPosition(ret);

				//ParallelLight->FaceTarget(ret, XMFLOAT3(0, 0.f, 40.f), XMFLOAT3(0.f, 0.f, 1.f));
			});
		}

		// 平面
		if (auto PlaneMesh = World->CreateActor<APlaneMesh>("ShadowMapTest::PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("ShadowMapTest::PlaneMeshComponent", 4.f, 3.f, 20, 20, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(30.f, 30.f, 1.f));
			if (auto PlaneMaterial = make_shared<CMaterial>())
			{
				PlaneMaterial->ResetGuid("ShadowMapTest::PlaneMateria");
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}
		// 球阵
		for (int height = 0; height < 1; ++height)
		{
			for (int row = 0; row < 2; ++row)
			{
				for (int col = 0; col < 3; ++col)
				{
					// 球的位置
					XMFLOAT3 Position(height * 7.f, col * 7.f - 7.0f, row * (-5.f) + 11.0f);

					int index = height * 100 + row * 10 + col;
					if (auto SphereMesh = World->CreateActor<ASphereMesh>("ShadowMapTest::SphereMesh_" + index))
					{
						SphereMesh->SetMeshComponent("ShadowMapTest::SphereMeshComponent_" + index, 2.f, 50, 50);
						SphereMesh->SetComponentPosition(Position);
						if (auto SphereMaterial = make_shared<CMaterial>())
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

		// 球旁边的一个立方体
		//if (ABoxMesh* BoxMesh = World->CreateActor<ABoxMesh>("ShadowMapTest::BoxMesh"))
		//{
		//	BoxMesh->SetMeshComponent("ShadowMapTest::BoxMeshComponent", 3, 3, 3);
		//	BoxMesh->SetComponentPosition(XMFLOAT3(0.f, 12.f, 1.f));
		//	if (CMaterial* BoxMeshMaterial = make_shared<CMaterial>())
		//	{
		//		BoxMeshMaterial->ResetGuid("ShadowMapTest::BoxMeshMaterial");//给创建的材质设置Guid
		//		BoxMeshMaterial->SetMaterialType(EMaterialType::Lambert);
		//		BoxMesh->SetSubMaterials(0, BoxMeshMaterial);
		//	}
		//}

		// 接受shadowmap的平面
		if (auto PlaneMesh = World->CreateActor<APlaneMesh>("ShadowMapTest::PlaneMesh_shadowmap"))
		{
			PlaneMesh->SetMeshComponent("ShadowMapTest::PlaneMeshComponent_shadowmap", 4.f, 3.f, 20, 20, EMeshComponentRenderLayerType::RENDERLAYER_ALPHATEST);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, -20.f, 7.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(4.f, 4.f, 4.f));
			PlaneMesh->SetComponentRotation(fvector_3d(0.f, -90.f, 0.f));
			if (auto PlaneMaterial = make_shared<CMaterial>())
			{
				PlaneMaterial->ResetGuid("ShadowMapTest::PlaneMateria_shadowmap");
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::ShadowMap);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}
	}
}