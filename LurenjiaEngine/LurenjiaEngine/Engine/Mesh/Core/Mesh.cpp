#include "Mesh.h"
#include "Material/Material.h"


AMesh::AMesh()
	: StaticMeshComponent(new CStaticMeshComponent())
{
	
}

void AMesh::Init()
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->Init();
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

void AMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->BuildMesh(InRenderingData);
	}
}

void AMesh::SetSubMaterials(const int& index, CMaterial* InMaterial)
{
	StaticMeshComponent->SetSubMaterials(index, InMaterial);
}

UINT AMesh::GetMaterialsCount() const
{
	return StaticMeshComponent->GetMaterialsCount();
}

const vector<CMaterial*>* AMesh::GetMaterials() const
{
	return StaticMeshComponent->GetMaterials();
}



