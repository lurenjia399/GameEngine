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

public:
	CMeshComponent* MeshComponet;							//代表的模型组件
	size_t MeshHash;										//代表的模型的哈希值

	UINT IndexSize;											//索引的数量
	UINT VertexSize;										//顶点的数量
	UINT IndexoffsetPosition;								//索引位置偏移
	UINT VertexoffsetPostion;								//顶点位置偏移
	UINT IndexTypeSize;										//索引类型size
	UINT VertexTypeSize;									//顶点类型size


	XMFLOAT4X4 WorldMatrix;									//m变换矩阵
	XMFLOAT4X4 TextureTransform;							//贴图的矩阵
	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;	//需要更新的资源

	int GeometryKey;										//this是属于哪一个Geometry的
	int MeshObjectOffset;									//当前数据在常量堆中的偏移

	FMeshRenderingData* MeshRenderingData;					//所有的mesh渲染数据,保存一下应该是方便读取
};