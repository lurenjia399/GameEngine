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

void CBoxMesh::CreateMeshRenderData(FMeshRenderingData& InRenderingData, const float& Inheight, const float& Inwidth, const float& Indepth)
{
	//构建顶点数据
	float cheight = Inheight / 2;
	float cwidth = Inwidth / 2;
	float cdepth = Indepth / 2;
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, -cheight, -cdepth),XMFLOAT4(Colors::LightGoldenrodYellow)));	//0
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, cheight, -cdepth),XMFLOAT4(Colors::MidnightBlue)));			//1
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, cheight, -cdepth),XMFLOAT4(Colors::Blue)));					//2
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, -cheight, -cdepth),XMFLOAT4(Colors::Red)));					//3
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, -cheight, cdepth),XMFLOAT4(Colors::PaleVioletRed)));			//4
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, cheight, cdepth),XMFLOAT4(Colors::BlueViolet)));				//5
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, cheight, cdepth),XMFLOAT4(Colors::Yellow)));					//6
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, -cheight, cdepth),XMFLOAT4(Colors::Green)));					//7

	//front
	InRenderingData.IndexData.emplace_back(0);InRenderingData.IndexData.emplace_back(1);InRenderingData.IndexData.emplace_back(2);
	InRenderingData.IndexData.emplace_back(0);InRenderingData.IndexData.emplace_back(2);InRenderingData.IndexData.emplace_back(3);
	//back
	InRenderingData.IndexData.emplace_back(7);InRenderingData.IndexData.emplace_back(6);InRenderingData.IndexData.emplace_back(5);
	InRenderingData.IndexData.emplace_back(7);InRenderingData.IndexData.emplace_back(5);InRenderingData.IndexData.emplace_back(4);
	//left
	InRenderingData.IndexData.emplace_back(4);InRenderingData.IndexData.emplace_back(5);InRenderingData.IndexData.emplace_back(1);
	InRenderingData.IndexData.emplace_back(4);InRenderingData.IndexData.emplace_back(1);InRenderingData.IndexData.emplace_back(0);
	//right
	InRenderingData.IndexData.emplace_back(3);InRenderingData.IndexData.emplace_back(2);InRenderingData.IndexData.emplace_back(6);
	InRenderingData.IndexData.emplace_back(3);InRenderingData.IndexData.emplace_back(6);InRenderingData.IndexData.emplace_back(7);
	//up
	InRenderingData.IndexData.emplace_back(1);InRenderingData.IndexData.emplace_back(5);InRenderingData.IndexData.emplace_back(6);
	InRenderingData.IndexData.emplace_back(1);InRenderingData.IndexData.emplace_back(6);InRenderingData.IndexData.emplace_back(2);
	//bottom
	InRenderingData.IndexData.emplace_back(4);InRenderingData.IndexData.emplace_back(0);InRenderingData.IndexData.emplace_back(3);
	InRenderingData.IndexData.emplace_back(4);InRenderingData.IndexData.emplace_back(3);InRenderingData.IndexData.emplace_back(7);
}

void CBoxMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}
