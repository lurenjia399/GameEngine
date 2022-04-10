#include "Mesh.h"

void FMesh::Init()
{
}

void FMesh::Draw(float DeltaTime)
{
}

void FMesh::BuildMesh(const FMeshRenderingData& InRenderingData)
{
	//��ȡ�������ݺ������Ĵ�С
	UINT VertexDataSize = InRenderingData.VertexData.size() * sizeof(FVertex);
	UINT IndexDataSize = InRenderingData.IndexData.size() * sizeof(uint16_t);

	//Ϊ�������������� ���ݸ�cpuBuffer
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

