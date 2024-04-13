#include "BoxMeshComponent.h"

CBoxMeshComponent::CBoxMeshComponent()
{
}

void CBoxMeshComponent::CreateMeshRenderData(FVertexRenderingData& InRenderingData, const float& Inheight, const float& Inwidth, const float& Indepth)
{
	// 世界中创建立方体
	// x轴指向右边，y轴指向上边，z轴指向里边
	/*
	* 从z轴负方向看向z轴正方向
	* 1 —— 2	5 —— 6
	* |    |    |    |
	* |    |	|    |
	* 0 —— 3	4 —— 7
	*/
	//构建顶点数据
	float cheight = Inheight / 2;
	float cwidth = Inwidth / 2;
	float cdepth = Indepth / 2;
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, -cheight, -cdepth), XMFLOAT4(Colors::LightGoldenrodYellow)));	//0
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, cheight, -cdepth), XMFLOAT4(Colors::MidnightBlue)));			//1
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, cheight, -cdepth), XMFLOAT4(Colors::Blue)));					//2
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, -cheight, -cdepth), XMFLOAT4(Colors::Red)));					//3
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, -cheight, cdepth), XMFLOAT4(Colors::PaleVioletRed)));			//4
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(-cwidth, cheight, cdepth), XMFLOAT4(Colors::BlueViolet)));				//5
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, cheight, cdepth), XMFLOAT4(Colors::Yellow)));					//6
	InRenderingData.VertexData.emplace_back(FVertex(XMFLOAT3(cwidth, -cheight, cdepth), XMFLOAT4(Colors::Green)));					//7

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

void CBoxMeshComponent::BuildKey(size_t& OutHashKey, const float& Inheight, const float& Inwidth, const float& Indepth)
{
	std::hash<float> floatHash;

	OutHashKey = floatHash(Inheight);
	OutHashKey += floatHash(Inwidth);
	OutHashKey += floatHash(Indepth);
}
