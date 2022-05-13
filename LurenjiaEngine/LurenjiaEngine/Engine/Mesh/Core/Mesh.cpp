#include "Mesh.h"
#include "Material/Material.h"


AMesh::AMesh()
{
	Materials.emplace_back(CreateObject<CMaterial>("MeshMaterial"));
}

void AMesh::Init()
{
	
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
}

UINT AMesh::GetMaterialsCount() const
{
	return Materials.size();
}
