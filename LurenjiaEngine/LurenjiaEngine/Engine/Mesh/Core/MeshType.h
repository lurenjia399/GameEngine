#pragma once
#include "../../LurenjiaEngine.h"
#include "../../Math/EngineMath.h"
struct FVertex
{
	//FVertex(const XMFLOAT3& InPos, XMFLOAT4 InColor = XMFLOAT4(Colors::White));
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor);

	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT3 Normal;
};

/// <summary>
/// ģ����Ⱦ����Ҫ������
/// Ҳ���Ƕ����������������
/// </summary>
struct FMeshRenderingData
{
	vector<FVertex> VertexData;
	vector<uint16_t> IndexData;
};