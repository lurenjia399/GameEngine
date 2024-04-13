#include "TextureTest.h"
#include "../../Actor/Mesh/Core/Material/Material.h"
#include "../../Actor/Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Actor/Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"

namespace TextureTest
{
	void TextureTest::BuildTextureTestData()
	{
		shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
		shared_ptr<CWorld> World = WindowsEngine->GetRenderingEngine()->GetWorld();
		if (auto PlaneMesh = World->CreateActor<APlaneMesh>("PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("PlaneMeshComponent", 4.f, 3.f, 20, 20);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(30.f, 30.f, 1.f));
			if (auto PlaneMaterial = make_shared<CMaterial>())
			{
				PlaneMaterial->ResetGuid("PlaneMateria");//给创建的材质设置Guid
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}

		//平行光
		if (auto ParallelLight = World->CreateActor<AParallelLight>("AParallelLight"))
		{
			ParallelLight->SetLightIntensity(XMFLOAT3(1.0f, 1.0f, 1.0f));
			ParallelLight->SetPosition(XMFLOAT3(-20.f, 0.f, 40.f));
			/*ParallelLight->SetRotateFunction([=](float time) ->void
			{
				float angle_speed = PI;
				float angle = time * angle_speed;
				XMFLOAT3 cur_pos = ParallelLight->GetPosition();
				XMFLOAT3 ret = cur_pos;
				float s = sin(angle);
				float c = cos(angle);

				ret = XMFLOAT3(cur_pos.x * c - cur_pos.y * s, cur_pos.x * s + cur_pos.y * c, cur_pos.z);
				ParallelLight->SetPosition(ret);

				ParallelLight->FaceTarget(ret, XMFLOAT3(0, 0.f, 40.f), XMFLOAT3(0.f, 0.f, 1.f));
			});*/
		}

		if (auto SphereMesh_1 = World->CreateActor<ASphereMesh>("SphereMesh_1"))
		{
			SphereMesh_1->SetMeshComponent("SphereMesh_1Component", 2.f, 50, 50);
			SphereMesh_1->SetComponentPosition(XMFLOAT3(0.f, -5.f, 2.f));
			//SphereMesh_1->SetComponentScale(XMFLOAT3(0.5f, 1.f, 1.f));
			if (auto SphereMaterial_1 = make_shared<CMaterial>())
			{
				SphereMaterial_1->ResetGuid("SphereMaterial_1");//给创建的材质设置Guid

				SphereMaterial_1->SetMaterialTextureMapKey("Wood2");
				SphereMaterial_1->SetSpecularColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
				SphereMaterial_1->SetRoughness(0.2f);
				SphereMaterial_1->SetMaterialType(EMaterialType::HalfLambert);
				SphereMaterial_1->SetMaterialFresnelF0(XMFLOAT3(0.05f, 0.05f, 0.05f));
				SphereMesh_1->SetSubMaterials(0, SphereMaterial_1);
				

			}
		}
		if (auto SphereMesh_2 = World->CreateActor<ASphereMesh>("SphereMesh_2"))
		{
			SphereMesh_2->SetMeshComponent("SphereMesh_2Component", 2.f, 50, 50);
			SphereMesh_2->SetComponentPosition(XMFLOAT3(0.f, 0.f, 2.f));
			
			if (auto SphereMaterial_2 = make_shared<CMaterial>())
			{
				SphereMaterial_2->ResetGuid("SphereMaterial_2");//给创建的材质设置Guid
				SphereMaterial_2->SetMaterialTextureMapKey("Wood2");
				SphereMaterial_2->SetMaterialNormalMapKey("Wood2_Nor");
				SphereMaterial_2->SetSpecularColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
				SphereMaterial_2->SetRoughness(0.2f);
				SphereMaterial_2->SetMaterialType(EMaterialType::HalfLambert);
				SphereMesh_2->SetSubMaterials(0, SphereMaterial_2);

			}
		}
		if (auto SphereMesh_3 = World->CreateActor<ASphereMesh>("SphereMesh_3"))
		{
			SphereMesh_3->SetMeshComponent("SphereMesh_32Component", 2.f, 50, 50);
			SphereMesh_3->SetComponentPosition(XMFLOAT3(0.f, 5.f, 2.f));
			if (auto SphereMaterial_3 = make_shared<CMaterial>())
			{
				SphereMaterial_3->ResetGuid("SphereMaterial_3");//给创建的材质设置Guid
				SphereMaterial_3->SetMaterialTextureMapKey("Wood2");
				SphereMaterial_3->SetMaterialNormalMapKey("Wood2_Nor");
				SphereMaterial_3->SetMaterialSpecularMapKey("Wood2_SPEC");
				SphereMaterial_3->SetRoughness(0.2f);
				SphereMaterial_3->SetMaterialType(EMaterialType::HalfLambert);
				SphereMesh_3->SetSubMaterials(0, SphereMaterial_3);

			}
		}
	}
}

