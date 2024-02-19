#include "ComputeShaderTest.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Mesh/BoxMesh.h"

// 记得去LurenjiaEngine.h里开启宏


namespace ComputeShaderTest
{
	void ComputeShaderTest::BuildComputeShaderTestData()
	{
		CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
		CWorld* World = WindowsEngine->GetRenderingEngine()->GetWorld();

		// cubeMap
		if (ABoxMesh* CubeMesh = World->CreateActor<ABoxMesh>("ComputeShaderTest::ABoxMesh"))
		{
			CubeMesh->SetMeshComponent("ComputeShaderTest::ABoxMeshComponent", 10, 10, 10, EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND);
			CubeMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, 0.f));
			CubeMesh->SetComponentScale(XMFLOAT3(100.f, 100.f, 100.f));
			if (CMaterial* CubeMaterial = new CMaterial())
			{
				CubeMaterial->ResetGuid("ComputeShaderTest::CubeMaterial");//给创建的材质设置Guid
				CubeMaterial->SetMaterialTextureMapKey("x1_CubeMap");
				CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
				CubeMesh->SetSubMaterials(0, CubeMaterial);
			}
		}


		//平行光
		if (AParallelLight* ParallelLight = World->CreateActor<AParallelLight>("ComputeShaderTest::AParallelLight"))
		{
			ParallelLight->SetLightIntensity(XMFLOAT3(0.5f, 0.5f, 0.5f));
			ParallelLight->SetPosition(XMFLOAT3(-20.f, 0.f, 40.f));
			ParallelLight->SetRotateFunction([=](float time) ->void
			{
				//float angle_speed = PI;
				//float angle = time * angle_speed;
				//XMFLOAT3 cur_pos = ParallelLight->GetPosition();
				//XMFLOAT3 ret = cur_pos;
				//float s = sin(angle);
				//float c = cos(angle);

				//ret = XMFLOAT3(cur_pos.x * c - cur_pos.y * s, cur_pos.x * s + cur_pos.y * c, cur_pos.z);
				//ParallelLight->SetPosition(ret);

				//ParallelLight->FaceTarget(ret, XMFLOAT3(0, 0.f, 40.f), XMFLOAT3(0.f, 0.f, 1.f));
			});
		}

		// 平面
		if (APlaneMesh* PlaneMesh = World->CreateActor<APlaneMesh>("ComputeShaderTest::PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("ComputeShaderTest::PlaneMeshComponent", 4.f, 3.f, 20, 20, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(30.f, 30.f, 1.f));
			if (CMaterial* PlaneMaterial = new CMaterial())
			{
				PlaneMaterial->ResetGuid("ComputeShaderTest::PlaneMateria");
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}

		// 平面贴图
		if (APlaneMesh* PlaneMesh1 = World->CreateActor<APlaneMesh>("ComputeShaderTest::PlaneMesh1"))
		{
			PlaneMesh1->SetMeshComponent("ComputeShaderTest::PlaneMeshComponent", 4.f, 3.f, 20, 20, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			PlaneMesh1->SetComponentPosition(XMFLOAT3(0.f, -20.f, 7.f));
			PlaneMesh1->SetComponentScale(XMFLOAT3(4.f, 4.f, 4.f));
			PlaneMesh1->SetComponentRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* PlaneMaterial = new CMaterial())
			{
				PlaneMaterial->ResetGuid("ComputeShaderTest::PlaneMateria");
				PlaneMaterial->SetMaterialTextureMapKey("Wood2");
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::Lambert);
				PlaneMesh1->SetSubMaterials(0, PlaneMaterial);
			}
		}
	}
}