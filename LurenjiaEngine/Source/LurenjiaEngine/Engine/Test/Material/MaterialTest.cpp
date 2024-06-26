#include "MaterialTest.h"
#include "../../Actor/Mesh/Core/Material/Material.h"
#include "../../Actor/Mesh/PlaneMesh.h"
#include "../../Actor/Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Actor/Mesh/BoxMesh.h"

namespace MaterialTest
{
	void MaterialTest::BuildMaterialTestData()
	{
		shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
		shared_ptr<CWorld> World = WindowsEngine->GetRenderingEngine()->GetWorld();
		// 注意这个天空盒，我们渲染流程开始的LoadTexture方法中就将天空盒的贴图资源传递到了SRV中
		// 也就是静态反射就会生效，和这里创建出天空盒Actor无关，这里只是能看到天空盒的贴图样子
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

		if (auto PlaneMesh = World->CreateActor<APlaneMesh>("PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("PlaneMeshComponent", 4.f, 3.f, 20, 20);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(30.f, 1.f, 30.f));
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
			ParallelLight->SetRotateFunction([=](float time) ->void
			{
				float angle_speed = (float)PI;
				float angle = time * angle_speed;
				XMFLOAT3 cur_pos = ParallelLight->GetPosition();
				XMFLOAT3 ret = cur_pos;
				float s = sin(angle);
				float c = cos(angle);
				
				ret = XMFLOAT3( cur_pos.x * c - cur_pos.y * s, cur_pos.x * s + cur_pos.y * c, cur_pos.z);
				ParallelLight->SetPosition(ret);

				ParallelLight->FaceTarget(ret, XMFLOAT3(0, 0.f, 40.f), XMFLOAT3(0.f, 0.f, 1.f));
			});
		}
		// EMaterialType::Lambert
		if (auto SphereMesh_1 = World->CreateActor<ASphereMesh>("SphereMesh_1"))
		{
			SphereMesh_1->SetMeshComponent("SphereMesh_1", 2.f, 20, 20);
			SphereMesh_1->SetComponentPosition(XMFLOAT3(0.f, -7.f, 2.f));
			if (auto SphereMaterial_1 = make_shared<CMaterial>())
			{
				SphereMaterial_1->ResetGuid("SphereMaterial_1");//给创建的材质设置Guid
				SphereMaterial_1->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_1->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_1->SetMaterialType(EMaterialType::Lambert);
				SphereMesh_1->SetSubMaterials(0, SphereMaterial_1);
			}
		}
		// EMaterialType::HalfLambert
		if (auto SphereMesh_2 = World->CreateActor<ASphereMesh>("SphereMesh_2"))
		{
			SphereMesh_2->SetMeshComponent("SphereMesh_2Component", 2.f, 20, 20);
			SphereMesh_2->SetComponentPosition(XMFLOAT3(0.f, -2.f, 2.f));
			if (auto SphereMaterial_2 = make_shared<CMaterial>())
			{
				SphereMaterial_2->ResetGuid("SphereMaterial_2");//给创建的材质设置Guid
				SphereMaterial_2->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)); //112 / 255.f, 20 / 255.f, 102 / 255.f, 1.0f));
				SphereMaterial_2->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_2->SetMaterialType(EMaterialType::HalfLambert);
				SphereMesh_2->SetSubMaterials(0, SphereMaterial_2);
			}
		}
		// EMaterialType::Phone
		if (auto SphereMesh_3 = World->CreateActor<ASphereMesh>("SphereMesh_3"))
		{
			SphereMesh_3->SetMeshComponent("SphereMesh_3Component", 2.f, 20, 20);
			SphereMesh_3->SetComponentPosition(XMFLOAT3(0.f, 3.f, 2.f));
			if (auto SphereMaterial_3 = make_shared<CMaterial>())
			{
				SphereMaterial_3->ResetGuid("SphereMaterial_3");//给创建的材质设置Guid
				SphereMaterial_3->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)); //191.f / 255.f, 173.f / 255.f, 111.f / 255.f, 1.0f));
				SphereMaterial_3->SetMaterialType(EMaterialType::Phone);
				SphereMaterial_3->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_3->SetRoughness(0.8f);
				SphereMesh_3->SetSubMaterials(0, SphereMaterial_3);
			}
		}
		// EMaterialType::BlinnPhone
		if (auto SphereMesh_4 = World->CreateActor<ASphereMesh>("SphereMesh_4"))
		{
			SphereMesh_4->SetMeshComponent("SphereMesh_4Component", 2.f, 20, 20);
			SphereMesh_4->SetComponentPosition(XMFLOAT3(0.f, 8.f, 2.f));
			if (auto SphereMaterial_4 = make_shared<CMaterial>())
			{
				SphereMaterial_4->ResetGuid("SphereMaterial_4");//给创建的材质设置Guid
				SphereMaterial_4->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)); //220.f / 255.f, 223.f / 255.f, 227.f / 255.f, 1.f));
				SphereMaterial_4->SetMaterialType(EMaterialType::BlinnPhone);
				SphereMaterial_4->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_4->SetRoughness(0.8f);
				SphereMesh_4->SetSubMaterials(0, SphereMaterial_4);
			}
		}
		// EMaterialType::WrapLight
		if (auto SphereMesh_5 = World->CreateActor<ASphereMesh>("SphereMesh_5"))
		{
			SphereMesh_5->SetMeshComponent("SphereMesh_5Component", 2.f, 20, 20);
			SphereMesh_5->SetComponentPosition(XMFLOAT3(0.f, -7.f, 7.f));
			if (auto SphereMaterial_5 = make_shared<CMaterial>())
			{
				SphereMaterial_5->ResetGuid("SphereMaterial_5");//给创建的材质设置Guid
				SphereMaterial_5->SetBaseColor(XMFLOAT4(221.f / 255.f, 154.f / 255.f, 127.f / 255.f, 1.0f));
				SphereMaterial_5->SetMaterialType(EMaterialType::WrapLight);
				SphereMaterial_5->SetSpecularColor(XMFLOAT3(0.2f, 0.2f, 0.2f));
				SphereMesh_5->SetSubMaterials(0, SphereMaterial_5);
			}
		}
		// EMaterialType::MinnaertLight
		if (auto SphereMesh_6 = World->CreateActor<ASphereMesh>("SphereMesh_6"))
		{
			SphereMesh_6->SetMeshComponent("SphereMesh_6Component", 2.f, 20, 20);
			SphereMesh_6->SetComponentPosition(XMFLOAT3(0.f, -2.f, 7.f));
			if (auto SphereMaterial_6 = make_shared<CMaterial>())
			{
				SphereMaterial_6->ResetGuid("SphereMaterial_6");//给创建的材质设置Guid
				SphereMaterial_6->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_6->SetMaterialType(EMaterialType::MinnaertLight);
				SphereMaterial_6->SetSpecularColor(XMFLOAT3(0.2f, 0.2f, 0.2f));
				SphereMaterial_6->SetRoughness(1.2f);
				SphereMesh_6->SetSubMaterials(0, SphereMaterial_6);
			}
		}
		// EMaterialType::BandedLight
		if (auto SphereMesh_7 = World->CreateActor<ASphereMesh>("SphereMesh_7"))
		{
			SphereMesh_7->SetMeshComponent("SphereMesh_7Component", 2.f, 20, 20);
			SphereMesh_7->SetComponentPosition(XMFLOAT3(0.f, -2.f, 12.f));
			if (auto SphereMaterial_7 = make_shared<CMaterial>())
			{
				SphereMaterial_7->ResetGuid("SphereMaterial_7");//给创建的材质设置Guid
				SphereMaterial_7->SetBaseColor(XMFLOAT4(247.f / 255.f, 150.f / 255.f, 85.f / 255.f, 1.0f));
				SphereMaterial_7->SetMaterialType(EMaterialType::BandedLight);
				SphereMesh_7->SetSubMaterials(0, SphereMaterial_7);
			}
		}
		// EMaterialType::GradualBanded
		if (auto SphereMesh_8 = World->CreateActor<ASphereMesh>("SphereMesh_8"))
		{
			SphereMesh_8->SetMeshComponent("SphereMesh_8Component", 2.f, 20, 20);
			SphereMesh_8->SetComponentPosition(XMFLOAT3(0.f, 3.f, 12.f));
			if (auto SphereMaterial_8 = make_shared<CMaterial>())
			{
				SphereMaterial_8->ResetGuid("SphereMaterial_8");//给创建的材质设置Guid
				SphereMaterial_8->SetBaseColor(XMFLOAT4(247.f / 255.f, 150.f / 255.f, 85.f / 255.f, 1.0f));
				SphereMaterial_8->SetMaterialType(EMaterialType::GradualBanded);
				SphereMesh_8->SetSubMaterials(0, SphereMaterial_8);
			}
		}
		// EMaterialType::FinalBanded
		if (auto SphereMesh_9 = World->CreateActor<ASphereMesh>("SphereMesh_9"))
		{
			SphereMesh_9->SetMeshComponent("SphereMesh_9Component", 2.f, 20, 20);
			SphereMesh_9->SetComponentPosition(XMFLOAT3(0.f, 8.f, 12.f));
			if (auto SphereMaterial_9 = make_shared<CMaterial>())
			{
				SphereMaterial_9->ResetGuid("SphereMaterial_9");//给创建的材质设置Guid
				SphereMaterial_9->SetBaseColor(XMFLOAT4(247.f / 255.f, 150.f / 255.f, 85.f / 255.f, 1.0f));
				SphereMaterial_9->SetMaterialType(EMaterialType::FinalBanded);
				SphereMesh_9->SetSubMaterials(0, SphereMaterial_9);
			}
		}
		// EMaterialType::Back
		if (auto SphereMesh_10 = World->CreateActor<ASphereMesh>("SphereMesh_10"))
		{
			SphereMesh_10->SetMeshComponent("SphereMesh_10Component", 2.f, 20, 20);
			SphereMesh_10->SetComponentPosition(XMFLOAT3(0.f, -7.f, 17.f));
			if (auto SphereMaterial_10 = make_shared<CMaterial>())
			{
				SphereMaterial_10->ResetGuid("SphereMaterial_10");//给创建的材质设置Guid
				SphereMaterial_10->SetBaseColor(XMFLOAT4(2.f / 255.f, 214.f / 255.f, 17.f / 255.f, 1.0f));
				SphereMaterial_10->SetMaterialType(EMaterialType::Back);
				SphereMaterial_10->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_10->SetRoughness(0.2f);
				SphereMesh_10->SetSubMaterials(0, SphereMaterial_10);
			}
		}
		// EMaterialType::AnisotropyKajiyaKay
		if (auto SphereMesh_12 = World->CreateActor<ASphereMesh>("SphereMesh_12"))
		{
			SphereMesh_12->SetMeshComponent("SphereMesh_12Component", 2.f, 20, 20);
			SphereMesh_12->SetComponentPosition(XMFLOAT3(0.f, -2.f, 17.f));
			if (auto SphereMaterial_12 = make_shared<CMaterial>())
			{
				SphereMaterial_12->ResetGuid("SphereMaterial_12");//给创建的材质设置Guid
				SphereMaterial_12->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_12->SetMaterialType(EMaterialType::AnisotropyKajiyaKay);
				SphereMesh_12->SetSubMaterials(0, SphereMaterial_12);
			}
		}
		// EMaterialType::OrenNayar
		if (auto SphereMesh_13 = World->CreateActor<ASphereMesh>("SphereMesh_13"))
		{
			SphereMesh_13->SetMeshComponent("SphereMesh_13Component", 2.f, 20, 20);
			SphereMesh_13->SetComponentPosition(XMFLOAT3(0.f, 3.f, 17.f));
			if (auto SphereMaterial_13 = make_shared<CMaterial>())
			{
				SphereMaterial_13->ResetGuid("SphereMaterial_13");//给创建的材质设置Guid

				SphereMaterial_13->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_13->SetMaterialType(EMaterialType::OrenNayar);
				SphereMaterial_13->SetRoughness(2.f);
				SphereMesh_13->SetSubMaterials(0, SphereMaterial_13);
			}
		}
		// EMaterialType::Fresnel
		if (auto SphereMesh_14 = World->CreateActor<ASphereMesh>("SphereMesh_14"))
		{
			SphereMesh_14->SetMeshComponent("SphereMesh_14Component", 2.f, 20, 20);
			SphereMesh_14->SetComponentPosition(XMFLOAT3(0.f, -7.f, 22.f));
			if (auto SphereMaterial_14 = make_shared<CMaterial>())
			{
				SphereMaterial_14->ResetGuid("SphereMaterial_5");//给创建的材质设置Guid
				SphereMaterial_14->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_14->SetMaterialType(EMaterialType::Fresnel);
				SphereMaterial_14->SetRoughness(0.15f);
				SphereMesh_14->SetSubMaterials(0, SphereMaterial_14);
			}
		}
		// 线框渲染
		if (auto SphereMesh_15 = World->CreateActor<ASphereMesh>("SphereMesh_15"))
		{
			SphereMesh_15->SetMeshComponent("SphereMesh_15Component", 2.f, 20, 20);
			SphereMesh_15->SetComponentPosition(XMFLOAT3(0.f, -2.f, 22.f));
			if (auto SphereMaterial_15 = make_shared<CMaterial>())
			{
				SphereMaterial_15->ResetGuid("SphereMaterial_15");//给创建的材质设置Guid
				SphereMaterial_15->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.0f));
				SphereMaterial_15->SetMaterialType(EMaterialType::BaseColor);
				SphereMaterial_15->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
				SphereMesh_15->SetSubMaterials(0, SphereMaterial_15);
			}
		}

		if (auto SphereMesh_16 = World->CreateActor<ASphereMesh>("SphereMesh_16"))
		{
			SphereMesh_16->SetMeshComponent("SphereMesh_16Component", 2.f, 20, 20);
			SphereMesh_16->SetComponentPosition(XMFLOAT3(0.f, 3, 22.f));
			if (auto SphereMaterial_16 = make_shared<CMaterial>())
			{
				SphereMaterial_16->ResetGuid("SphereMaterial_16");//给创建的材质设置Guid
				SphereMaterial_16->SetBaseColor(XMFLOAT4(0.1f, 0.7f, 0.5f, 1.0f));
				SphereMaterial_16->SetMaterialType(EMaterialType::WorldNormal);
				SphereMesh_16->SetSubMaterials(0, SphereMaterial_16);
			}
		}
		if (auto SphereMesh_17 = World->CreateActor<ASphereMesh>("SphereMesh_17"))
		{
			SphereMesh_17->SetMeshComponent("SphereMesh_17Component", 2.f, 20, 20);
			SphereMesh_17->SetComponentPosition(XMFLOAT3(0.f, 8, 22.f));
			if (auto SphereMaterial_17 = make_shared<CMaterial>())
			{
				SphereMaterial_17->ResetGuid("SphereMaterial_17");//给创建的材质设置Guid
				SphereMaterial_17->SetBaseColor(XMFLOAT4(0.1f, 0.7f, 0.5f, 1.0f));
				SphereMaterial_17->SetMaterialType(EMaterialType::Normal);
				SphereMesh_17->SetSubMaterials(0, SphereMaterial_17);
			}
		}
	}
}
