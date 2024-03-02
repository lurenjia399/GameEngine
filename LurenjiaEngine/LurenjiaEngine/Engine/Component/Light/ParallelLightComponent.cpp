#include "ParallelLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"

CParallelLightComponent::CParallelLightComponent()
	:LightMeshComponent(nullptr)
{

	//����ParallelLight��Mesh
	string ParalLelLightPath = "../LurenjiaEngine/Asset/SunMesh.obj";
	LightMeshComponent = GetMeshManage()->CreateCustomMeshComponent("ParallelLightMeshComponent", ParalLelLightPath);

	if (LightMeshComponent)
	{
		//��Ϊ ģ�ͱ���ĳ���������õ�transformation�����в���
		//���� �ڴ�����ʱ���һ����תƫ��
		// Ŀǰ����Ҫ
		//ParallelLightMeshComponent->SetRotation(fvector_3d(0.f, 0.f, -90.f));

		shared_ptr<CMaterial> material = LightMeshComponent->GetMaterials()[0];
		material->SetMaterialType(EMaterialType::BaseColor);
		material->SetMaterialDisplayStatusType(EMaterialDisplayStatusType::WireframeDisplay);
		material->SetBaseColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	}
#if SHOWLIGHTMESH
#endif
	

	SetLightType(ELightType::ParallelLight);
}

void CParallelLightComponent::SetPosition(const XMFLOAT3& InPosition)
{
	super::SetPosition(InPosition);
	LightMeshComponent->SetPosition(InPosition);
}

void CParallelLightComponent::SetRotation(const fvector_3d& InRotation)
{
	super::SetRotation(InRotation);
	LightMeshComponent->SetRotation(InRotation);
}

void CParallelLightComponent::SetScale(const XMFLOAT3& InScale)
{
	super::SetScale(InScale);
	LightMeshComponent->SetScale(InScale);
}
