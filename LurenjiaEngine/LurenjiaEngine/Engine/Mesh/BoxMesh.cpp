#include "BoxMesh.h"

void FBoxMesh::Init()
{
	Super::Init();
}

void FBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

FBoxMesh* FBoxMesh::CreateMesh()
{
	//构建顶点数据
	FMeshRenderingData BoxMeshInfo;
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(0.f, 0.f, 0.f),XMFLOAT4(Colors::White)));
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(0.f, 1.f, 0.f),XMFLOAT4(Colors::White)));
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(1.f, 1.f, 0.f),XMFLOAT4(Colors::White)));
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(1.f, 0.f, 0.f),XMFLOAT4(Colors::White)));
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(0.f, 0.f, 1.f),XMFLOAT4(Colors::White)));
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(0.f, 1.f, 1.f),XMFLOAT4(Colors::White)));
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(1.f, 1.f, 1.f),XMFLOAT4(Colors::White)));
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(1.f, 0.f, 1.f),XMFLOAT4(Colors::White)));

	//bottom
	BoxMeshInfo.IndexData.emplace_back(0);BoxMeshInfo.IndexData.emplace_back(1);BoxMeshInfo.IndexData.emplace_back(2);
	BoxMeshInfo.IndexData.emplace_back(0);BoxMeshInfo.IndexData.emplace_back(2);BoxMeshInfo.IndexData.emplace_back(3);
	//top
	BoxMeshInfo.IndexData.emplace_back(7);BoxMeshInfo.IndexData.emplace_back(6);BoxMeshInfo.IndexData.emplace_back(5);
	BoxMeshInfo.IndexData.emplace_back(7);BoxMeshInfo.IndexData.emplace_back(5);BoxMeshInfo.IndexData.emplace_back(4);
	//front
	BoxMeshInfo.IndexData.emplace_back(3);BoxMeshInfo.IndexData.emplace_back(2);BoxMeshInfo.IndexData.emplace_back(6);
	BoxMeshInfo.IndexData.emplace_back(3);BoxMeshInfo.IndexData.emplace_back(6);BoxMeshInfo.IndexData.emplace_back(7);
	//back
	BoxMeshInfo.IndexData.emplace_back(1);BoxMeshInfo.IndexData.emplace_back(0);BoxMeshInfo.IndexData.emplace_back(4);
	BoxMeshInfo.IndexData.emplace_back(1);BoxMeshInfo.IndexData.emplace_back(4);BoxMeshInfo.IndexData.emplace_back(5);
	//left
	BoxMeshInfo.IndexData.emplace_back(0);BoxMeshInfo.IndexData.emplace_back(3);BoxMeshInfo.IndexData.emplace_back(7);
	BoxMeshInfo.IndexData.emplace_back(0);BoxMeshInfo.IndexData.emplace_back(7);BoxMeshInfo.IndexData.emplace_back(4);
	//right
	BoxMeshInfo.IndexData.emplace_back(2);BoxMeshInfo.IndexData.emplace_back(1);BoxMeshInfo.IndexData.emplace_back(5);
	BoxMeshInfo.IndexData.emplace_back(2);BoxMeshInfo.IndexData.emplace_back(5);BoxMeshInfo.IndexData.emplace_back(6);

	FBoxMesh* BoxMesh = new FBoxMesh();
	BoxMesh->BuildMesh(BoxMeshInfo);

	BoxMesh->Init();
	return BoxMesh;
}

void FBoxMesh::BuildMesh(const FMeshRenderingData& InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
