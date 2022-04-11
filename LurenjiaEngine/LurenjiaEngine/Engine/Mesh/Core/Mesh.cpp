#include "Mesh.h"

FMesh::FMesh():
	VertexSizeInBytes(0),
	VertexStrideInBytes(0),
	IndexSizeInBytes(0),
	IndexFormat(DXGI_FORMAT_R16_UINT),
	IndexSize(0)
{
}

void FMesh::Init()
{
}

void FMesh::Draw(float DeltaTime)
{
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
	if (GraphicsCommandList == nullptr)
	{
		Engine_Log_Error("FMesh::Draw GraphicsCommandList is nullptr");
		return;
	}
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW IndexBufferView = GetIndexBufferView();
	GraphicsCommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	GraphicsCommandList->IASetIndexBuffer(&IndexBufferView);

	//����ͼԪ��triandleList
	GraphicsCommandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/// <summary>
	/// [in] UINT IndexCountPerInstance,ÿ������ʵ��������������
	///	[in] UINT InstanceCount,		��Ҫ���Ƶ�ʵ������
	///	[in] UINT StartIndexLocation,	GPU��IndexBuffer��ȡ�ĵ�һ��indexλ��
	///	[in] INT  BaseVertexLocation,	��ÿ���������ƫ�ƣ��ڶ�ȡVertexBuffer֮ǰ
	///	[in] UINT StartInstanceLocation	��ÿ���������ƫ�ƣ��ڴ�VertexBuffer��ȡʵ��֮ǰ
	/// </summary>
	/// <param name="DeltaTime"></param>
	GraphicsCommandList->DrawIndexedInstanced(IndexSize, 1, 0, 0, 0);
}

void FMesh::BuildMesh(const FMeshRenderingData& InRenderingData)
{
	IndexSize = InRenderingData.IndexData.size();
	VertexStrideInBytes = sizeof(FVertex);
	//��ȡ�������ݺ������Ĵ�С
	VertexSizeInBytes = InRenderingData.VertexData.size() * VertexStrideInBytes;
	IndexSizeInBytes = IndexSize * sizeof(uint16_t);

	//Ϊ�������������� ���ݸ�cpuBuffer
	ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData.VertexData.data(), VertexSizeInBytes);
	ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData.IndexData.data(), IndexSizeInBytes);

	GPUVertexBufferPtr = ConstructDefaultBuffer(VertexBufferTempPtr, InRenderingData.VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer(IndexBufferTempPtr, InRenderingData.IndexData.data(), IndexSizeInBytes);
	if (GPUIndexBufferPtr == nullptr || GPUVertexBufferPtr == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh discover error");
		return;
	}
}

D3D12_VERTEX_BUFFER_VIEW FMesh::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV = {};
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	VBV.SizeInBytes = VertexSizeInBytes;
	VBV.StrideInBytes = VertexStrideInBytes;

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FMesh::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV = {};
	IBV.SizeInBytes = IndexSizeInBytes;
	IBV.Format = IndexFormat;
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();

	return IBV;
}

