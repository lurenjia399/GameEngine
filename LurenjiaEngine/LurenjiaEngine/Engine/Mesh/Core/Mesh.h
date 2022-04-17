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
	virtual void Init();					//重写父类纯虚函数
	virtual void PreDraw(float DeltaTime);	//重写父类虚函数
	virtual void Draw(float DeltaTime);		//重写父类纯虚函数
	virtual void PostDraw(float DeltaTime);	//重写父类虚函数

	virtual void BuildMesh(const FMeshRenderingData& InRenderingData);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:

	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU存放mesh的顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU存放mesh的索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh的顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh的索引缓冲区

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//临时mesh的顶点缓冲区
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//临时mesh的索引缓冲区

	ComPtr<ID3D12DescriptorHeap> CBVHeap;					//常量描述堆
	shared_ptr<FRenderingResourcesUpdate> objectConstants;	//常量缓冲区

	ComPtr<ID3D12RootSignature> RootSignature;				//根签名

	FShader VertexShader;									//顶点着色器
	FShader PixelShader;									//片元着色器
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		//着色器输入布局
	ComPtr<ID3D12PipelineState> PSO;						//管线状态
protected:
	
	UINT VertexSizeInBytes;									//顶点资源所占内存大小
	UINT VertexStrideInBytes;								//顶点步径大小（也就是每个顶点数据大小）

	UINT IndexSizeInBytes;									//索引资源所占内存大小
	DXGI_FORMAT IndexFormat;								//索引资源格式
	UINT IndexSize;											//顶点的数量

	XMFLOAT4X4 WorldMatrix;									//mvp变换矩阵
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectMatrix;
};