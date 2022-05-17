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


void AMesh::SetSubMaterials(const int& index, CMaterial* InMaterial)
{
	Materials[index] = InMaterial;
}

UINT AMesh::GetMaterialsCount() const
{
	return Materials.size();
}

const vector<CMaterial*>* AMesh::GetMaterials() const
{
	return &Materials;
}
