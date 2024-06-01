#include "SelectActorTest.h"
#include "../../Actor/Mesh/Core/Material/Material.h"
#include "../../Actor/Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Actor/Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Actor/Mesh/BoxMesh.h"
#include "../../Actor/Mesh/CylinderMesh.h"
#include "../../../Editor/SelectEditor/OperationHandle/OperationHandle_Move.h"


namespace SelectActorTest
{
	void SelectActorTest::BuildSelectActorTestData()
	{
		shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
		shared_ptr<CWorld> World = WindowsEngine->GetRenderingEngine()->GetWorld();

		// 选择箭头
		if (auto Arrow = World->CreateActor<AOperationHandle_Move>("AOperationHandle_Move"))
		{
			Arrow->CreateComponent();
			Arrow->SetPosition(XMFLOAT3(0,0,3.f));
			{
				shared_ptr<CMaterial> MaterialX = make_shared<CMaterial>();
				shared_ptr<CMaterial> MaterialY = make_shared<CMaterial>();
				shared_ptr<CMaterial> MaterialZ = make_shared<CMaterial>();

				MaterialX->ResetGuid("SelectActorTest::AOperationHandle_Move_XAxis");
				MaterialY->ResetGuid("SelectActorTest::AOperationHandle_Move_YAxis");
				MaterialZ->ResetGuid("SelectActorTest::AOperationHandle_Move_ZAxis");
				Arrow->SetSubMaterials(0, MaterialX, MaterialY, MaterialZ);

				Arrow->SetBaseColor();
			}
		}


		// cubeMap
		if (auto CubeMesh = World->CreateActor<ABoxMesh>("SelectActorTest::ABoxMesh"))
		{
			CubeMesh->SetMeshComponent("SelectActorTest::ABoxMeshComponent", 10, 10, 10, EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND);
			CubeMesh->SetPosition(XMFLOAT3(0.f, 0.f, 0.f));
			CubeMesh->SetScale(XMFLOAT3(100.f, 100.f, 100.f));
			if (auto CubeMaterial = make_shared<CMaterial>())
			{
				CubeMaterial->ResetGuid("SelectActorTest::CubeMaterial");//给创建的材质设置Guid
				CubeMaterial->SetMaterialTextureMapKey("x1_CubeMap");
				CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
				CubeMesh->SetSubMaterials(0, CubeMaterial);
			}
		}
		//平行光
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
					//ParallelLight->SetPosition(ret);
					//ParallelLight->FaceTarget(ret, XMFLOAT3(0, 0.f, 40.f), XMFLOAT3(0.f, 0.f, 1.f));
				});
		}

		// 平面
		if (auto PlaneMesh = World->CreateActor<APlaneMesh>("SelectActorTest::PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("SelectActorTest::PlaneMeshComponent", 4.f, 3.f, 20, 20, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			PlaneMesh->SetPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetScale(XMFLOAT3(30.f, 1.f, 30.f));
			if (auto PlaneMaterial = make_shared<CMaterial>())
			{
				PlaneMaterial->ResetGuid("SelectActorTest::PlaneMateria");
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
			PlaneMesh->SetPickup(false); // 关掉拾取功能
		}
		
		// 球
		if (auto SphereMesh_1 = World->CreateActor<ASphereMesh>("SelectActorTest::SphereMesh"))
		{
			SphereMesh_1->SetMeshComponent("SelectActorTest::SphereMesh", 2.f, 20, 20);
			SphereMesh_1->SetPosition(XMFLOAT3(0.f, -7.f, 2.f));
			if (auto SphereMaterial_1 = make_shared<CMaterial>())
			{
				SphereMaterial_1->ResetGuid("SelectActorTest::SphereMaterial");//给创建的材质设置Guid
				SphereMaterial_1->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_1->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_1->SetMaterialType(EMaterialType::Lambert);
				SphereMesh_1->SetSubMaterials(0, SphereMaterial_1);
			}
			//SphereMesh_1->SetPickup(false); // 关掉拾取功能
		}

		// 立方体
		if (auto CubeMesh = World->CreateActor<ABoxMesh>("SelectActorTest::ABoxMesh2"))
		{
			CubeMesh->SetMeshComponent("SelectActorTest::ABoxMeshComponent2", 4, 4, 4, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			CubeMesh->SetPosition(XMFLOAT3(0.f, 0.f, 3.f));
			CubeMesh->SetScale(XMFLOAT3(1.f, 1.f, 1.f));
			if (auto CubeMaterial = make_shared<CMaterial>())
			{
				CubeMaterial->ResetGuid("SelectActorTest::CubeMaterial2");//给创建的材质设置Guid
				CubeMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				CubeMaterial->SetMaterialType(EMaterialType::HalfLambert);
				CubeMesh->SetSubMaterials(0, CubeMaterial);
			}
			//CubeMesh->SetPickup(false);
		}

		// 柱状mesh
		if (auto CylinderMesh = World->CreateActor<ACylinderMesh>("SelectActorTest::ACylinderMesh"))
		{
			CylinderMesh->SetMeshComponent("SelectActorTest::ACylinderMeshComponent", 6, 4, 20, 10, 20, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			CylinderMesh->SetPosition(XMFLOAT3(0.f, 8.f, 3.f));
			CylinderMesh->SetScale(XMFLOAT3(1.f, 1.f, 1.f));
			if (auto CylinderMaterial = make_shared<CMaterial>())
			{
				CylinderMaterial->ResetGuid("SelectActorTest::CylinderMaterial");//给创建的材质设置Guid
				CylinderMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				CylinderMaterial->SetMaterialType(EMaterialType::HalfLambert);
				CylinderMesh->SetSubMaterials(0, CylinderMaterial);
			}
			//CylinderMesh->SetPickup(false);
		}
		XMFLOAT3 a = XMFLOAT3(1, 1, 1);
		XMVECTOR WorldDirection = XMLoadFloat3(&a);
		XMFLOAT3 b = XMFLOAT3(1, 2, 3);
		XMVECTOR SelectActorDirection = XMLoadFloat3(&b);

		XMVECTOR v1_Corss_v2 = DirectX::XMVector3Cross(WorldDirection, SelectActorDirection);
		XMVECTOR v1_Corss_v2_vector = DirectX::XMVector3Length(v1_Corss_v2);
		XMVECTOR WorldDirection_vector = DirectX::XMVector3Length(WorldDirection);
		XMVECTOR SelectActorDirection_vector = DirectX::XMVector3Length(SelectActorDirection);
		float v1_Corss_v2_len = DirectX::XMVector3LengthSq(v1_Corss_v2).m128_f32[0];
		float WorldDirection_len = DirectX::XMVector3LengthSq(WorldDirection).m128_f32[0];
		float SelectActorDirection_len = DirectX::XMVector3LengthSq(SelectActorDirection).m128_f32[0];
		XMVECTOR v1_dot_v2 = DirectX::XMVector3Dot(WorldDirection, SelectActorDirection);


		XMVECTOR SelectActorDirection2 = XMLoadFloat3(&b);
		//float t1 = DirectX::XMVector3Dot(
		//	DirectX::XMVector3Cross(DirectX::XMVectorSubtract(SelectActorPostion, WorldOriginPoint),
		//		SelectActorDirection),
		//	v1_Corss_v2).m128_f32[0]
		//	/ (v1_Corss_v2_len * v1_Corss_v2_len);
		//XMVECTOR ResultPos = DirectX::XMVectorAdd(WorldOriginPoint, DirectX::XMVectorScale(WorldDirection, t1));
		//XMFLOAT3 ResultPostion = XMFLOAT3();
		//XMStoreFloat3(&ResultPostion, ResultPos);
		//SelectedActor_SharedPtr->SetPosition(ResultPostion);

	}
}