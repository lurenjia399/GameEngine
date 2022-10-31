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
				PlaneMaterial->ResetGuid("ReflectTest::PlaneMateria");//�������Ĳ�������Guid
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
				SphereMaterial->SetMaterialType(EMaterialType::BlinnPhone);
				SphereMaterial->SetRoughness(0.1f);
				SphereMaterial->SetMaterialFresnelF0(XMFLOAT3(0.05f, 0.05f, 0.05f));
				SphereMesh->SetSubMaterials(0, SphereMaterial);
			}
		}
	}
}