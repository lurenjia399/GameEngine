#include "ReflectTest.h"
#include "../../Actor/Mesh/Core/Material/Material.h"
//#include "../Actor/Mesh/Core/Material/Material.h"
#include "../../Actor/Mesh/SphereMesh.h"
#include "../../Core/World.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Actor/Mesh/PlaneMesh.h"
#include "../../Actor/Light/ParallelLight.h"
#include "../../Actor/Mesh/BoxMesh.h"

namespace ReflectTest
{
	void ReflectTest::BuildReflectTestData()
	{
		shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
		shared_ptr<CWorld> World = WindowsEngine->GetRenderingEngine()->GetWorld();
		// 天空盒
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
		// 平面
		if (auto PlaneMesh = World->CreateActor<APlaneMesh>("ReflectTest::PlaneMesh"))
		{
			PlaneMesh->SetMeshComponent("ReflectTest::PlaneMeshComponent", 4.f, 3.f, 20, 20);
			PlaneMesh->SetComponentPosition(XMFLOAT3(0.f, 0.f, -2.f));
			PlaneMesh->SetComponentScale(XMFLOAT3(30.f, 30.f, 1.f));
			if (auto PlaneMaterial = make_shared<CMaterial>())
			{
				PlaneMaterial->ResetGuid("ReflectTest::PlaneMateria");//给创建的材质设置Guid
				PlaneMaterial->SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
				PlaneMaterial->SetMaterialType(EMaterialType::HalfLambert);
				PlaneMesh->SetSubMaterials(0, PlaneMaterial);
			}
		}
		if (auto ParallelLight = World->CreateActor<AParallelLight>("AParallelLight"))
		{
			ParallelLight->SetLightIntensity(XMFLOAT3(1.0f, 1.0f, 1.0f));
			ParallelLight->SetPosition(XMFLOAT3(-30.f, 0.f, 0.f));
			ParallelLight->SetRotation(fvector_3d(0.f, 0.f, 90.0f));
		}
		// 静态反射球，只反射包围盒
		if (auto SphereMesh = World->CreateActor<ASphereMesh>("ReflectTest::SphereMesh"))
		{
			SphereMesh->SetMeshComponent("ReflectTest::SphereMeshComponent", 2.f, 50, 50);
			SphereMesh->SetComponentPosition(XMFLOAT3(0.f, -5.f, 2.f));
			//SphereMesh->SetComponentScale(XMFLOAT3(1.f, 1.f, 1.5f));
			if (auto SphereMaterial = make_shared<CMaterial>())
			{
				SphereMaterial->ResetGuid("ReflectTest::SphereMaterial");
				//SphereMaterial->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.0f));
				SphereMaterial->SetMaterialType(EMaterialType::Phone);
				SphereMaterial->SetRoughness(0.1f);
				SphereMaterial->SetMaterialFresnelF0(XMFLOAT3(0.2f, 0.2f, 0.2f));
				SphereMesh->SetSubMaterials(0, SphereMaterial);
			}
		}
		// 动态反射球
		if (auto SphereMesh = World->CreateActor<ASphereMesh>("DynamicReflectTest::SphereMesh"))
		{
			SphereMesh->SetMeshComponent("DynamicReflectTest::SphereMeshComponent", 2.f, 50, 50, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUEREFLECT);
			SphereMesh->SetComponentPosition(XMFLOAT3(0.f, 5.f, 7.f));
			
			if (auto SphereMaterial = make_shared<CMaterial>())
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
		// 验证反射是否正确的颜色球
		if (auto SphereMesh = World->CreateActor<ASphereMesh>("DynamicReflectTest::SphereMeshs"))
		{
			SphereMesh->SetMeshComponent("DynamicReflectTest::SphereMeshComponents", 2.f, 50, 50, EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE);
			SphereMesh->SetComponentPosition(XMFLOAT3(0.f, 10.f, 2.f));
			
			if (auto SphereMaterial = make_shared<CMaterial>())
			{
				SphereMaterial->ResetGuid("DynamicReflectTest::SphereMaterialss");
				SphereMaterial->SetBaseColor(XMFLOAT4(1.f, 0.f, 1.f, 1.0f));
				SphereMaterial->SetMaterialType(EMaterialType::Lambert);
				SphereMaterial->SetRoughness(0.1f);
				//SphereMaterial->SetMaterialFresnelF0(XMFLOAT3(0.05f, 0.05f, 0.05f));
				SphereMesh->SetSubMaterials(0, SphereMaterial);
			}
		}
		if (auto SphereMesh_17 = World->CreateActor<ASphereMesh>("SphereMesh_17"))
		{
			SphereMesh_17->SetMeshComponent("SphereMesh_17Component", 2.f, 20, 20);
			SphereMesh_17->SetComponentPosition(XMFLOAT3(0.f, -2, 22.f));
			
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