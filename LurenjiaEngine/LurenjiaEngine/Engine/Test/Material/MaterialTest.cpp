#include "MaterialTest.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"

namespace MaterialTest
{
	void MaterialTest::BuildMaterialTestData()
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
			SphereMesh_1->SetMeshComponent("SphereMesh_1", 2.f, 20, 20);
			SphereMesh_1->SetComponentPosition(XMFLOAT3(0.f, -7.f, 2.f));
			if (CMaterial* SphereMaterial_1 = new CMaterial())
			{
				SphereMaterial_1->ResetGuid("SphereMaterial_1");//给创建的材质设置Guid
				SphereMaterial_1->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_1->SetMaterialType(EMaterialType::Lambert);
				SphereMesh_1->SetSubMaterials(0, SphereMaterial_1);
			}
		}
		if (ASphereMesh* SphereMesh_2 = World->CreateActor<ASphereMesh>("SphereMesh_2"))
		{
			SphereMesh_2->SetMeshComponent("SphereMesh_2Component", 2.f, 20, 20);
			SphereMesh_2->SetComponentPosition(XMFLOAT3(0.f, -2.f, 2.f));
			if (CMaterial* SphereMaterial_2 = new CMaterial())
			{
				SphereMaterial_2->ResetGuid("SphereMaterial_2");//给创建的材质设置Guid
				SphereMaterial_2->SetBaseColor(XMFLOAT4(112 / 255.f, 242 / 255.f, 132 / 255.f, 1.0f));
				SphereMaterial_2->SetMaterialType(EMaterialType::HalfLambert);
				SphereMesh_2->SetSubMaterials(0, SphereMaterial_2);
			}
		}
		if (ASphereMesh* SphereMesh_3 = World->CreateActor<ASphereMesh>("SphereMesh_3"))
		{
			SphereMesh_3->SetMeshComponent("SphereMesh_3Component", 2.f, 20, 20);
			SphereMesh_3->SetComponentPosition(XMFLOAT3(0.f, 3.f, 2.f));
			if (CMaterial* SphereMaterial_3 = new CMaterial())
			{
				SphereMaterial_3->ResetGuid("SphereMaterial_3");//给创建的材质设置Guid
				SphereMaterial_3->SetBaseColor(XMFLOAT4(191.f / 255.f, 173.f / 255.f, 111.f / 255.f, 1.0f));
				SphereMaterial_3->SetMaterialType(EMaterialType::Phone);
				SphereMaterial_3->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_3->SetRoughness(0.8f);
				SphereMesh_3->SetSubMaterials(0, SphereMaterial_3);
			}
		}
		if (ASphereMesh* SphereMesh_4 = World->CreateActor<ASphereMesh>("SphereMesh_4"))
		{
			SphereMesh_4->SetMeshComponent("SphereMesh_4Component", 2.f, 20, 20);
			SphereMesh_4->SetComponentPosition(XMFLOAT3(0.f, 8.f, 2.f));
			if (CMaterial* SphereMaterial_4 = new CMaterial())
			{
				SphereMaterial_4->ResetGuid("SphereMaterial_4");//给创建的材质设置Guid
				SphereMaterial_4->SetBaseColor(XMFLOAT4(220.f / 255.f, 223.f / 255.f, 227.f / 255.f, 1.f));
				SphereMaterial_4->SetMaterialType(EMaterialType::BlinnPhone);
				SphereMaterial_4->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_4->SetRoughness(0.8f);
				SphereMesh_4->SetSubMaterials(0, SphereMaterial_4);
			}
		}
		if (ASphereMesh* SphereMesh_5 = World->CreateActor<ASphereMesh>("SphereMesh_5"))
		{
			SphereMesh_5->SetMeshComponent("SphereMesh_5Component", 2.f, 20, 20);
			SphereMesh_5->SetComponentPosition(XMFLOAT3(0.f, -7.f, 7.f));
			if (CMaterial* SphereMaterial_5 = new CMaterial())
			{
				SphereMaterial_5->ResetGuid("SphereMaterial_5");//给创建的材质设置Guid
				SphereMaterial_5->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_5->SetMaterialType(EMaterialType::Fresnel);
				SphereMaterial_5->SetRoughness(0.15f);
				SphereMesh_5->SetSubMaterials(0, SphereMaterial_5);
			}
		}
		if (ASphereMesh* SphereMesh_6 = World->CreateActor<ASphereMesh>("SphereMesh_6"))
		{
			SphereMesh_6->SetMeshComponent("SphereMesh_6Component", 2.f, 20, 20);
			SphereMesh_6->SetComponentPosition(XMFLOAT3(0.f, -2.f, 7.f));
			if (CMaterial* SphereMaterial_6 = new CMaterial())
			{
				SphereMaterial_6->ResetGuid("SphereMaterial_6");//给创建的材质设置Guid
				SphereMaterial_6->SetBaseColor(XMFLOAT4(221.f / 255.f, 154.f / 255.f, 127.f / 255.f, 1.0f));
				SphereMaterial_6->SetMaterialType(EMaterialType::WrapLight);
				SphereMaterial_6->SetSpecularColor(XMFLOAT3(0.2f, 0.2f, 0.2f));
				SphereMesh_6->SetSubMaterials(0, SphereMaterial_6);
			}
		}
		if (ASphereMesh* SphereMesh_7 = World->CreateActor<ASphereMesh>("SphereMesh_7"))
		{
			SphereMesh_7->SetMeshComponent("SphereMesh_7Component", 2.f, 20, 20);
			SphereMesh_7->SetComponentPosition(XMFLOAT3(0.f, 3.f, 7.f));
			if (CMaterial* SphereMaterial_7 = new CMaterial())
			{
				SphereMaterial_7->ResetGuid("SphereMaterial_7");//给创建的材质设置Guid
				SphereMaterial_7->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_7->SetMaterialType(EMaterialType::MinnaertLight);
				SphereMaterial_7->SetSpecularColor(XMFLOAT3(0.2f, 0.2f, 0.2f));
				SphereMaterial_7->SetRoughness(1.2f);
				SphereMesh_7->SetSubMaterials(0, SphereMaterial_7);
			}
		}
		if (ASphereMesh* SphereMesh_8 = World->CreateActor<ASphereMesh>("SphereMesh_8"))
		{
			SphereMesh_8->SetMeshComponent("SphereMesh_8Component", 2.f, 20, 20);
			SphereMesh_8->SetComponentPosition(XMFLOAT3(0.f, 8.f, 7.f));
			if (CMaterial* SphereMaterial_8 = new CMaterial())
			{
				SphereMaterial_8->ResetGuid("SphereMaterial_8");//给创建的材质设置Guid
				SphereMaterial_8->SetBaseColor(XMFLOAT4(0.7f, 0.7f, 1.4f, 1.0f));
				SphereMaterial_8->SetMaterialType(EMaterialType::BandedLight);
				SphereMesh_8->SetSubMaterials(0, SphereMaterial_8);
			}
		}
		if (ASphereMesh* SphereMesh_9 = World->CreateActor<ASphereMesh>("SphereMesh_9"))
		{
			SphereMesh_9->SetMeshComponent("SphereMesh_9Component", 2.f, 20, 20);
			SphereMesh_9->SetComponentPosition(XMFLOAT3(0.f, -7.f, 12.f));
			if (CMaterial* SphereMaterial_9 = new CMaterial())
			{
				SphereMaterial_9->ResetGuid("SphereMaterial_9");//给创建的材质设置Guid
				SphereMaterial_9->SetBaseColor(XMFLOAT4(247.f / 255.f, 150.f / 255.f, 85.f / 255.f, 1.0f));
				SphereMaterial_9->SetMaterialType(EMaterialType::GradualBanded);
				SphereMesh_9->SetSubMaterials(0, SphereMaterial_9);
			}
		}
		if (ASphereMesh* SphereMesh_10 = World->CreateActor<ASphereMesh>("SphereMesh_10"))
		{
			SphereMesh_10->SetMeshComponent("SphereMesh_10Component", 2.f, 20, 20);
			SphereMesh_10->SetComponentPosition(XMFLOAT3(0.f, -2.f, 12.f));
			if (CMaterial* SphereMaterial_10 = new CMaterial())
			{
				SphereMaterial_10->ResetGuid("SphereMaterial_10");//给创建的材质设置Guid
				SphereMaterial_10->SetBaseColor(XMFLOAT4(0.5f, 0.9f, 0.7f, 1.0f));
				SphereMaterial_10->SetMaterialType(EMaterialType::FinalBanded);
				SphereMaterial_10->SetRoughness(0.15f);
				SphereMesh_10->SetSubMaterials(0, SphereMaterial_10);
			}
		}
		if (ASphereMesh* SphereMesh_11 = World->CreateActor<ASphereMesh>("SphereMesh_11"))
		{
			SphereMesh_11->SetMeshComponent("SphereMesh_11Component", 2.f, 20, 20);
			SphereMesh_11->SetComponentPosition(XMFLOAT3(0.f, 3.f, 12.f));
			if (CMaterial* SphereMaterial_11 = new CMaterial())
			{
				SphereMaterial_11->ResetGuid("SphereMaterial_11");//给创建的材质设置Guid
				SphereMaterial_11->SetBaseColor(XMFLOAT4(2.f / 255.f, 214.f / 255.f, 17.f / 255.f, 1.0f));
				SphereMaterial_11->SetMaterialType(EMaterialType::Back);
				SphereMaterial_11->SetSpecularColor(XMFLOAT3(0.1f, 0.1f, 0.1f));
				SphereMaterial_11->SetRoughness(0.2f);
				SphereMesh_11->SetSubMaterials(0, SphereMaterial_11);
			}
		}
		if (ASphereMesh* SphereMesh_12 = World->CreateActor<ASphereMesh>("SphereMesh_12"))
		{
			SphereMesh_12->SetMeshComponent("SphereMesh_12Component", 2.f, 20, 20);
			SphereMesh_12->SetComponentPosition(XMFLOAT3(0.f, 8.f, 12.f));
			if (CMaterial* SphereMaterial_12 = new CMaterial())
			{
				SphereMaterial_12->ResetGuid("SphereMaterial_12");//给创建的材质设置Guid
				SphereMaterial_12->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_12->SetMaterialType(EMaterialType::AnisotropyKajiyaKay);
				SphereMesh_12->SetSubMaterials(0, SphereMaterial_12);
			}
		}
		if (ASphereMesh* SphereMesh_13 = World->CreateActor<ASphereMesh>("SphereMesh_13"))
		{
			SphereMesh_13->SetMeshComponent("SphereMesh_13Component", 2.f, 20, 20);
			SphereMesh_13->SetComponentPosition(XMFLOAT3(0.f, -7.f, 17.f));
			if (CMaterial* SphereMaterial_13 = new CMaterial())
			{
				SphereMaterial_13->ResetGuid("SphereMaterial_13");//给创建的材质设置Guid

				SphereMaterial_13->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				SphereMaterial_13->SetMaterialType(EMaterialType::OrenNayar);
				SphereMaterial_13->SetRoughness(2.f);
				SphereMesh_13->SetSubMaterials(0, SphereMaterial_13);
			}
		}
		if (ASphereMesh* SphereMesh_14 = World->CreateActor<ASphereMesh>("SphereMesh_14"))
		{
			SphereMesh_14->SetMeshComponent("SphereMesh_14Component", 2.f, 20, 20);
			SphereMesh_14->SetComponentPosition(XMFLOAT3(0.f, -2.f, 17.f));
			if (CMaterial* SphereMaterial_14 = new CMaterial())
			{
				SphereMaterial_14->ResetGuid("SphereMaterial_14");//给创建的材质设置Guid
				SphereMaterial_14->SetBaseColor(XMFLOAT4(1.5f, 1.3f, 1.5f, 1.0f));
				SphereMaterial_14->SetMaterialType(EMaterialType::BaseColor);
				SphereMaterial_14->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
				SphereMesh_14->SetSubMaterials(0, SphereMaterial_14);
			}
		}
		if (ASphereMesh* SphereMesh_15 = World->CreateActor<ASphereMesh>("SphereMesh_15"))
		{
			SphereMesh_15->SetMeshComponent("SphereMesh_15Component", 2.f, 20, 20);
			SphereMesh_15->SetComponentPosition(XMFLOAT3(0.f, 3.f, 17.f));
			if (CMaterial* SphereMaterial_15 = new CMaterial())
			{
				SphereMaterial_15->ResetGuid("SphereMaterial_15");//给创建的材质设置Guid
				SphereMaterial_15->SetBaseColor(XMFLOAT4(0.1f, 0.7f, 0.5f, 1.0f));
				SphereMaterial_15->SetMaterialType(EMaterialType::BaseColor);
				SphereMaterial_15->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::PointDisplay);
				SphereMesh_15->SetSubMaterials(0, SphereMaterial_15);
			}
		}
		if (ASphereMesh* SphereMesh_16 = World->CreateActor<ASphereMesh>("SphereMesh_16"))
		{
			SphereMesh_16->SetMeshComponent("SphereMesh_16Component", 2.f, 20, 20);
			SphereMesh_16->SetComponentPosition(XMFLOAT3(0.f, -7, 22.f));
			if (CMaterial* SphereMaterial_16 = new CMaterial())
			{
				SphereMaterial_16->ResetGuid("SphereMaterial_16");//给创建的材质设置Guid
				SphereMaterial_16->SetBaseColor(XMFLOAT4(0.1f, 0.7f, 0.5f, 1.0f));
				SphereMaterial_16->SetMaterialType(EMaterialType::WorldNormal);
				SphereMesh_16->SetSubMaterials(0, SphereMaterial_16);
			}
		}
		if (ASphereMesh* SphereMesh_17 = World->CreateActor<ASphereMesh>("SphereMesh_17"))
		{
			SphereMesh_17->SetMeshComponent("SphereMesh_17Component", 2.f, 20, 20);
			SphereMesh_17->SetComponentPosition(XMFLOAT3(0.f, -2, 22.f));
			if (CMaterial* SphereMaterial_17 = new CMaterial())
			{
				SphereMaterial_17->ResetGuid("SphereMaterial_17");//给创建的材质设置Guid
				SphereMaterial_17->SetBaseColor(XMFLOAT4(0.1f, 0.7f, 0.5f, 1.0f));
				SphereMaterial_17->SetMaterialType(EMaterialType::Normal);
				SphereMesh_17->SetSubMaterials(0, SphereMaterial_17);
			}
		}
	}
}
