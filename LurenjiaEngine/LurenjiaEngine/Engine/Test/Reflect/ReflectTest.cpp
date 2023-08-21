#include "ReflectTest.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Mesh/PlaneMesh.h"

namespace ReflectTest
{
	void ReflectTest::BuildReflectTestData()
	{
		CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
		CWorld* World = WindowsEngine->GetRenderingEngine()->GetWorld();
		if (APlaneMesh* PlaneMesh = World->CreateActor<APlaneMesh>("ReflectTest::PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("ReflectTest::PlaneMeshComponent", 4.f, 3.f, 20, 20);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(30.f, 30.f, 1.f));
			if (CMaterial* PlaneMaterial = new CMaterial())
			{
				PlaneMaterial->ResetGuid("ReflectTest::PlaneMateria");//给创建的材质设置Guid
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}

		if (ASphereMesh* SphereMesh = World->CreateActor<ASphereMesh>("ReflectTest::SphereMesh"))
		{
			SphereMesh->SetMeshComponent("ReflectTest::SphereMeshComponent", 2.f, 50, 50);
			SphereMesh->SetComponentPosition(XMFLOAT3(0.f, -5.f, 2.f));
			if (CMaterial* SphereMaterial = new CMaterial())
			{
				SphereMaterial->ResetGuid("ReflectTest::SphereMaterial");
				//SphereMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.0f));
				SphereMaterial->SetMaterialType(EMaterialType::Phone);
				SphereMaterial->SetRoughness(0.1f);
				SphereMaterial->SetMaterialFresnelF0(XMFLOAT3(0.2f, 0.2f, 0.2f));
				SphereMesh->SetSubMaterials(0, SphereMaterial);
			}
		}
		if (ASphereMesh* SphereMesh = World->CreateActor<ASphereMesh>("DynamicReflectTest::SphereMesh"))
		{
			//主要测试用的反射球
			SphereMesh->SetMeshComponent("DynamicReflectTest::SphereMeshComponent", 2.f, 50, 50, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUEREFLECT);
			SphereMesh->SetComponentPosition(XMFLOAT3(0.f, 5.f, 7.f));
			if (CMaterial* SphereMaterial = new CMaterial())
			{
				SphereMaterial->ResetGuid("DynamicReflectTest::SphereMaterial");
				//SphereMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.0f));
				SphereMaterial->SetMaterialType(EMaterialType::Phone);
				SphereMaterial->SetRoughness(0.1f);
				SphereMaterial->SetMaterialFresnelF0(XMFLOAT3(0.05f, 0.05f, 0.05f));
				SphereMaterial->SetDynamicReflection(true);
				SphereMesh->SetSubMaterials(0, SphereMaterial);
			}
		}
		if (ASphereMesh* SphereMesh = World->CreateActor<ASphereMesh>("DynamicReflectTest::SphereMeshs"))
		{
			SphereMesh->SetMeshComponent("DynamicReflectTest::SphereMeshComponents", 2.f, 50, 50, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			SphereMesh->SetComponentPosition(XMFLOAT3(0.f, 10.f, 2.f));
			if (CMaterial* SphereMaterial = new CMaterial())
			{
				SphereMaterial->ResetGuid("DynamicReflectTest::SphereMaterialss");
				SphereMaterial->SetBaseColor(XMFLOAT4(1.f, 0.f, 1.f, 1.0f));
				SphereMaterial->SetMaterialType(EMaterialType::Default);
				//SphereMaterial->SetRoughness(0.1f);
				//SphereMaterial->SetMaterialFresnelF0(XMFLOAT3(0.05f, 0.05f, 0.05f));
				SphereMesh->SetSubMaterials(0, SphereMaterial);
			}
		}
		//if (ASphereMesh* SphereMesh_16 = World->CreateActor<ASphereMesh>("SphereMesh_16"))
		//{
		//	SphereMesh_16->SetMeshComponent("SphereMesh_16Component", 2.f, 20, 20);
		//	SphereMesh_16->SetComponentPosition(XMFLOAT3(0.f, -7, 22.f));
		//	if (CMaterial* SphereMaterial_16 = new CMaterial())
		//	{
		//		SphereMaterial_16->ResetGuid("SphereMaterial_16");//给创建的材质设置Guid
		//		SphereMaterial_16->SetBaseColor(XMFLOAT4(0.1f, 0.7f, 0.5f, 1.0f));
		//		SphereMaterial_16->SetMaterialType(EMaterialType::WorldNormal);
		//		SphereMesh_16->SetSubMaterials(0, SphereMaterial_16);
		//	}
		//}
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