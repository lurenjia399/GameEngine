#include "Mesh.h"
#include "Material/Material.h"


AMesh::AMesh()
	: MeshComponent(new CMeshComponent())
{
	MeshComponent->ResetGuid("AMesh::MeshComponent");
}

void AMesh::Init()
{
	if (MeshComponent)
	{
		MeshComponent->Init();
	}
}

void AMesh::PreDraw(float DeltaTime)
{
	
}

void AMesh::Draw(float DeltaTime)
{
	
}

void AMesh::PostDraw(float DeltaTime)
{
	
}

void AMesh::SetPosition(const XMFLOAT3& InPosition)
{
	TransformationComponent->SetPosition(InPosition);
}

void AMesh::SetRotation(const fvector_3d& InRotation)
{
	TransformationComponent->SetRotation(InRotation);
}

void AMesh::SetScale(const XMFLOAT3& InScale)
{
	TransformationComponent->SetScale(InScale);
}

void AMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	if (MeshComponent)
	{
		MeshComponent->BuildMesh(InRenderingData);
	}
}

void AMesh::SetMeshComponent(CMeshComponent* InMeshComponent)
{
	MeshComponent = InMeshComponent;
}

void AMesh::SetMeshComponentLayerType(EMeshComponentRenderingLayerType InType)
{
	MeshComponent->SetMeshComponentLayerType(InType);
}

void AMesh::SetComponentPosition(const XMFLOAT3& InPosition)
{
	MeshComponent->SetPosition(InPosition);
}

void AMesh::SetComponentRotation(const fvector_3d& InRotation)
{
	MeshComponent->SetRotation(InRotation);
}

void AMesh::SetComponentScale(const XMFLOAT3& InScale)
{
	MeshComponent->SetScale(InScale);
}

void AMesh::SetSubMaterials(const int& index, CMaterial* InMaterial)
{
	MeshComponent->SetSubMaterials(index, InMaterial);
}

UINT AMesh::GetMaterialsCount() const
{
	return MeshComponent->GetMaterialsCount();
}

const vector<CMaterial*>* AMesh::GetMaterials() const
{
	return MeshComponent->GetMaterials();
}



