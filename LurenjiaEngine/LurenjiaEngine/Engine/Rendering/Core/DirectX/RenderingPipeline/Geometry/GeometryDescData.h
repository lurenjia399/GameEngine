#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Mesh/Core/Mesh.h"

struct FGeometryDescData : public IDirectXDeviceInterface_struct
{
public:
	FGeometryDescData();

public:
	UINT GetVertexSizeInBytes() const;						//获取顶点数据大小
	UINT GetIndexSizeInBytes() const;						//获取索引数据大小


	CMesh* Mesh;											//代表的模型

public:
	DXGI_FORMAT IndexFormat;								//索引资源格式

	UINT IndexSize;											//索引的数量
	UINT VertexSize;										//顶点的数量
	UINT IndexoffsetPosition;								//索引位置偏移
	UINT VertexoffsetPostion;								//顶点位置偏移
	UINT IndexTypeSize;										//索引类型size
	UINT VertexTypeSize;									//顶点类型size


	XMFLOAT4X4 WorldMatrix;									//mvp变换矩阵
	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;	//需要更新的资源
};