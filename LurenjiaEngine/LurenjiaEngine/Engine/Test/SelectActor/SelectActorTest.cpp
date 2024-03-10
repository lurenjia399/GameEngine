#include "SelectActorTest.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Mesh/BoxMesh.h"


namespace SelectActorTest
{
	void SelectActorTest::BuildSelectActorTestData()
	{
		shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
		shared_ptr<CWorld> World = WindowsEngine->GetRenderingEngine()->GetWorld();

		// cubeMap
		//if (auto CubeMesh = World->CreateActor<ABoxMesh>("ABoxMesh"))
		//{
		//	CubeMesh->SetMeshComponent("ABoxMeshComponent", 10, 10, 10, EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND);
		//	CubeMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, 0.f));
		//	CubeMesh->SetComponentScale(XMFLOAT3(100.f, 100.f, 100.f));
		//	if (auto CubeMaterial = make_shared<CMaterial>())
		//	{
		//		CubeMaterial->ResetGuid("CubeMaterial");//给创建的材质设置Guid
		//		CubeMaterial->SetMaterialTextureMapKey("x1_CubeMap");
		//		CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
		//		CubeMesh->SetSubMaterials(0, CubeMaterial);
		//	}
		//}
		//平行光
		//if (auto ParallelLight = World->CreateActor<AParallelLight>("AParallelLight"))
		//{
		//	ParallelLight->SetLightIntensity(XMFLOAT3(1.0f, 1.0f, 1.0f));
		//	ParallelLight->SetPosition(XMFLOAT3(-20.f, 0.f, 40.f));
		//	ParallelLight->SetRotateFunction([=](float time) ->void
		//		{
		//			float angle_speed = (float)PI;
		//			float angle = time * angle_speed;
		//			XMFLOAT3 cur_pos = ParallelLight->GetPosition();
		//			XMFLOAT3 ret = cur_pos;
		//			float s = sin(angle);
		//			float c = cos(angle);
		//			ret = XMFLOAT3(cur_pos.x * c - cur_pos.y * s, cur_pos.x * s + cur_pos.y * c, cur_pos.z);
		//			//ParallelLight->SetPosition(ret);
		//			//ParallelLight->FaceTarget(ret, XMFLOAT3(0, 0.f, 40.f), XMFLOAT3(0.f, 0.f, 1.f));
		//		});
		//}

		

		// 平面
		if (auto PlaneMesh = World->CreateActor<APlaneMesh>("ShadowMapTest::PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("ShadowMapTest::PlaneMeshComponent", 4.f, 3.f, 20, 20, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			//PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, -12.f, 0.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(50.f, 1.f, 50.f));
			if (auto PlaneMaterial = make_shared<CMaterial>())
			{
				PlaneMaterial->ResetGuid("ShadowMapTest::PlaneMateria");
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}
		
		//if (auto CubeMesh = World->CreateActor<ABoxMesh>("ABoxMesh3"))
		//{
		//	CubeMesh->SetMeshComponent("ABoxMeshComponent3", 10, 10, 10, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
		//	CubeMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, 0.f));
		//	CubeMesh->SetComponentScale(XMFLOAT3(1.f, 30.f, 1.f));
		//	if (auto CubeMaterial = make_shared<CMaterial>())
		//	{
		//		CubeMaterial->ResetGuid("CubeMaterial3");//给创建的材质设置Guid
		//		//CubeMaterial->SetMaterialTextureMapKey("x1_CubeMap3");
		//		CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
		//		CubeMesh->SetSubMaterials(0, CubeMaterial);
		//	}
		//}
	}
}