#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"

class FMesh :public IRenderingInterface
{
public:
	virtual void Init();				//��д���ി�麯��
	virtual void Draw(float DeltaTime);	//��д���ി�麯��

	virtual void BuildMesh(const FMeshRenderingData& InRenderingData);
protected:

	ComPtr<ID3DBlob> CPUVertexBufferPtr;			//CPU���mesh�Ķ��㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				//CPU���mesh������������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		//GPUmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		//GPUmesh������������

	ComPtr<ID3D12Resource> VertexBufferTempPtr;		//��ʱmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> IndexBufferTempPtr;		//��ʱmesh������������
};