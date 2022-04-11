#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"

class FMesh :public IRenderingInterface
{
public:
	FMesh();
	virtual void Init();				//��д���ി�麯��
	virtual void Draw(float DeltaTime);	//��д���ി�麯��

	virtual void BuildMesh(const FMeshRenderingData& InRenderingData);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:

	ComPtr<ID3DBlob> CPUVertexBufferPtr;			//CPU���mesh�Ķ��㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				//CPU���mesh������������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		//GPUmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		//GPUmesh������������

	ComPtr<ID3D12Resource> VertexBufferTempPtr;		//��ʱmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> IndexBufferTempPtr;		//��ʱmesh������������

protected:
	
	UINT VertexSizeInBytes;							//������Դ��ռ�ڴ��С
	UINT VertexStrideInBytes;						//���㲽����С��Ҳ����ÿ���������ݴ�С��

	UINT IndexSizeInBytes;							//������Դ��ռ�ڴ��С
	DXGI_FORMAT IndexFormat;						//������Դ��ʽ
	UINT IndexSize;								//���������
};