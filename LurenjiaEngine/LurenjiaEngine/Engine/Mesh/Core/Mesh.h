#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"

class FMesh :public IRenderingInterface
{
public:
	FMesh();
	virtual void Init();				//重写父类纯虚函数
	virtual void Draw(float DeltaTime);	//重写父类纯虚函数

	virtual void BuildMesh(const FMeshRenderingData& InRenderingData);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:

	ComPtr<ID3DBlob> CPUVertexBufferPtr;			//CPU存放mesh的顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				//CPU存放mesh的索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		//GPUmesh的顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		//GPUmesh的索引缓冲区

	ComPtr<ID3D12Resource> VertexBufferTempPtr;		//临时mesh的顶点缓冲区
	ComPtr<ID3D12Resource> IndexBufferTempPtr;		//临时mesh的索引缓冲区

protected:
	
	UINT VertexSizeInBytes;							//顶点资源所占内存大小
	UINT VertexStrideInBytes;						//顶点步径大小（也就是每个顶点数据大小）

	UINT IndexSizeInBytes;							//索引资源所占内存大小
	DXGI_FORMAT IndexFormat;						//索引资源格式
	UINT IndexSize;								//顶点的数量
};