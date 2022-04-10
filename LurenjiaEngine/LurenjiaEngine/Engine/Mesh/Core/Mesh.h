#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"

class FMesh :public IRenderingInterface
{
public:
	virtual void Init();				//重写父类纯虚函数
	virtual void Draw(float DeltaTime);	//重写父类纯虚函数

	virtual void BuildMesh(const FMeshRenderingData& InRenderingData);
protected:

	ComPtr<ID3DBlob> CPUVertexBufferPtr;			//CPU存放mesh的顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				//CPU存放mesh的索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		//GPUmesh的顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		//GPUmesh的索引缓冲区

	ComPtr<ID3D12Resource> VertexBufferTempPtr;		//临时mesh的顶点缓冲区
	ComPtr<ID3D12Resource> IndexBufferTempPtr;		//临时mesh的索引缓冲区
};