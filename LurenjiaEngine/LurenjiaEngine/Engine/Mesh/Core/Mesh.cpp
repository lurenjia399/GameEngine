#include "Mesh.h"

void FMesh::Init()
{
}

void FMesh::Draw(float DeltaTime)
{
}

void FMesh::BuildMesh(const FMeshRenderingData& InRenderingData)
{
	//获取顶点数据和索引的大小
	UINT VertexDataSize = InRenderingData.VertexData.size() * sizeof(FVertex);
	UINT IndexDataSize = InRenderingData.IndexData.size() * sizeof(uint16_t);

	//为将顶点索引数据 传递给cpuBuffer
	ANALYSIS_HRESULT(D3DCreateBlob(VertexDataSize, &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData.VertexData.data(), VertexDataSize);
	ANALYSIS_HRESULT(D3DCreateBlob(IndexDataSize, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData.IndexData.data(), IndexDataSize);

	GPUVertexBufferPtr = ConstructDefaultBuffer(VertexBufferTempPtr, InRenderingData.VertexData.data(), VertexDataSize);
	GPUIndexBufferPtr = ConstructDefaultBuffer(IndexBufferTempPtr, InRenderingData.IndexData.data(), IndexDataSize);
	if (GPUIndexBufferPtr == nullptr || GPUVertexBufferPtr == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh discover error");
		return;
	}
}

