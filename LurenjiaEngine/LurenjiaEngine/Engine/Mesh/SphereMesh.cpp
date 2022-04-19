#include "SphereMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"

void CSphereMesh::Init()
{
	Super::Init();
}

void CSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

CSphereMesh* CSphereMesh::CreateMesh()
{
	FMeshRenderingData SphereMeshInfo;



	CSphereMesh* SphereMesh = new CSphereMesh();
	SphereMesh->BuildMesh(&SphereMeshInfo);
	SphereMesh->ResetGuid("SphereMesh");

	SphereMesh->Init();
	return SphereMesh;
}

void CSphereMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
