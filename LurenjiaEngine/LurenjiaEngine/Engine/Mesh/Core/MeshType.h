#pragma once
#include "../../LurenjiaEngine.h"
#include "../../Math/EngineMath.h"
struct FVertex
{
	//FVertex(const XMFLOAT3& InPos, XMFLOAT4 InColor = XMFLOAT4(Colors::White));
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal = XMFLOAT3(0.f, 0.f, 1.f));

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
	UINT GetVertexSizeInBytes() const;						//��ȡ�������ݴ�С
	UINT GetIndexSizeInBytes() const;						//��ȡ�������ݴ�С
	vector<FVertex> VertexData;
	vector<uint16_t> IndexData;
};