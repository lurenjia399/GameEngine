#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;
	static XMFLOAT4X4 IdentityMatrix4x4();
};


class FMesh :public IRenderingInterface
{
public:
	FMesh();
	virtual void Init();					//��д���ി�麯��
	virtual void PreDraw(float DeltaTime);	//��д�����麯��
	virtual void Draw(float DeltaTime);		//��д���ി�麯��
	virtual void PostDraw(float DeltaTime);	//��д�����麯��

	virtual void BuildMesh(const FMeshRenderingData& InRenderingData);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:

	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU���mesh�Ķ��㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU���mesh������������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh������������

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//��ʱmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//��ʱmesh������������

	ComPtr<ID3D12DescriptorHeap> CBVHeap;					//����������
	shared_ptr<FRenderingResourcesUpdate> objectConstants;	//����������

	ComPtr<ID3D12RootSignature> RootSignature;				//��ǩ��

	FShader VertexShader;									//������ɫ��
	FShader PixelShader;									//ƬԪ��ɫ��
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		//��ɫ�����벼��
	ComPtr<ID3D12PipelineState> PSO;						//����״̬
protected:
	
	UINT VertexSizeInBytes;									//������Դ��ռ�ڴ��С
	UINT VertexStrideInBytes;								//���㲽����С��Ҳ����ÿ���������ݴ�С��

	UINT IndexSizeInBytes;									//������Դ��ռ�ڴ��С
	DXGI_FORMAT IndexFormat;								//������Դ��ʽ
	UINT IndexSize;											//���������

	XMFLOAT4X4 WorldMatrix;									//mvp�任����
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectMatrix;
};