#include "BoxMesh.h"
#include "../Core/CoreObject/CoreMinimalObject.h"

void CBoxMesh::Init()
{
	Super::Init();
}

void CBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

CBoxMesh* CBoxMesh::CreateMesh(float Inheight, float Inwidth, float Indepth)
{
	//构建顶点数据
	FMeshRenderingData BoxMeshInfo;
	float cheight = Inheight / 2;
	float cwidth = Inwidth / 2;
	float cdepth = Indepth / 2;
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, -cheight, -cdepth),XMFLOAT4(Colors::LightGoldenrodYellow)));	//0
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, cheight, -cdepth),XMFLOAT4(Colors::MidnightBlue)));			//1
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, cheight, -cdepth),XMFLOAT4(Colors::Bisque)));					//2
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, -cheight, -cdepth),XMFLOAT4(Colors::DarkMagenta)));			//3
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, -cheight, cdepth),XMFLOAT4(Colors::PaleVioletRed)));			//4
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, cheight, cdepth),XMFLOAT4(Colors::BlueViolet)));				//5
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, cheight, cdepth),XMFLOAT4(Colors::AliceBlue)));				//6
	BoxMeshInfo.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, -cheight, cdepth),XMFLOAT4(Colors::Violet)));					//7

	//front
	BoxMeshInfo.IndexData.emplace_back(0);BoxMeshInfo.IndexData.emplace_back(1);BoxMeshInfo.IndexData.emplace_back(2);
	BoxMeshInfo.IndexData.emplace_back(0);BoxMeshInfo.IndexData.emplace_back(2);BoxMeshInfo.IndexData.emplace_back(3);
	//back
	BoxMeshInfo.IndexData.emplace_back(7);BoxMeshInfo.IndexData.emplace_back(6);BoxMeshInfo.IndexData.emplace_back(5);
	BoxMeshInfo.IndexData.emplace_back(7);BoxMeshInfo.IndexData.emplace_back(5);BoxMeshInfo.IndexData.emplace_back(4);
	//left
	BoxMeshInfo.IndexData.emplace_back(4);BoxMeshInfo.IndexData.emplace_back(5);BoxMeshInfo.IndexData.emplace_back(1);
	BoxMeshInfo.IndexData.emplace_back(4);BoxMeshInfo.IndexData.emplace_back(1);BoxMeshInfo.IndexData.emplace_back(0);
	//right
	BoxMeshInfo.IndexData.emplace_back(3);BoxMeshInfo.IndexData.emplace_back(2);BoxMeshInfo.IndexData.emplace_back(6);
	BoxMeshInfo.IndexData.emplace_back(3);BoxMeshInfo.IndexData.emplace_back(6);BoxMeshInfo.IndexData.emplace_back(7);
	//up
	BoxMeshInfo.IndexData.emplace_back(1);BoxMeshInfo.IndexData.emplace_back(5);BoxMeshInfo.IndexData.emplace_back(6);
	BoxMeshInfo.IndexData.emplace_back(1);BoxMeshInfo.IndexData.emplace_back(6);BoxMeshInfo.IndexData.emplace_back(2);
	//bottom
	BoxMeshInfo.IndexData.emplace_back(4);BoxMeshInfo.IndexData.emplace_back(0);BoxMeshInfo.IndexData.emplace_back(3);
	BoxMeshInfo.IndexData.emplace_back(4);BoxMeshInfo.IndexData.emplace_back(3);BoxMeshInfo.IndexData.emplace_back(7);

	CBoxMesh* BoxMesh = new CBoxMesh();
	//CBoxMesh* BoxMesh = CreateObject<CBoxMesh>("BoxMesh"); 在静态方法中无法使用非静态方法
	BoxMesh->BuildMesh(&BoxMeshInfo);
	BoxMesh->ResetGuid("BoxMesh");

	BoxMesh->Init();
	return BoxMesh;
}

void CBoxMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
