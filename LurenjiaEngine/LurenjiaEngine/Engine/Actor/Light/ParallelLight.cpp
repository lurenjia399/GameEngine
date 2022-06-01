#include "ParallelLight.h"
#include "../../Manage/LightManage.h"
#include "../../Mesh/Core/MeshManage.h"

AParallelLight::AParallelLight()
	: ParallelLightComponent(nullptr)
	, ParallelLightMeshComponent(nullptr)
{
	//灯光的基类CLightComponent会自动将component添加到LightManage中
	ParallelLightComponent = CreateObject<CParallelLightComponent>("ParallelLightComponent");

	//创建ParallelLight的Mesh
	string ParalLelLightPath = "../LurenjiaEngine/Asset/Camera.obj";
	ParallelLightMeshComponent = GetMeshManage()->CreateCustomMeshComponent("ParallelLightMeshComponent", ParalLelLightPath);

	if (ParallelLightMeshComponent)
	{
		//因为 模型本身的朝向和我设置的transformation朝向有差异
		//所以 在创建的时候给一个旋转偏移
		ParallelLightMeshComponent->SetRotation(fvector_3d(0.f, 0.f, -90.f));

		CMaterial* material = (*ParallelLightMeshComponent->GetMaterials())[0];
		if (material)
		{
			material->SetMaterialType(EMaterialType::BaseColor);
			material->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
			material->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		}

	}
}

void AParallelLight::SetComponentPosition(const XMFLOAT3& InPosition)
{
	ParallelLightComponent->SetPosition(InPosition);
	ParallelLightMeshComponent->SetPosition(InPosition);
}

void AParallelLight::SetComponentRotation(const fvector_3d& InRotation)
{
	ParallelLightComponent->SetRotation(InRotation);
	ParallelLightMeshComponent->SetRotation(InRotation);
}

void AParallelLight::SetComponentScale(const XMFLOAT3& InScale)
{
	ParallelLightComponent->SetScale(InScale);
	ParallelLightMeshComponent->SetScale(InScale);
}
