#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "GeometryDescData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/DirectXConstBufferView.h"

struct FGeometry : public IDirectXDeviceInterface_struct
{
public:
	bool isExitRenderingData(CMesh* InKey);
	void BuildMeshDescData(CMesh* InMesh, const FMeshRenderingData* MeshRenderData);
	void BuildMeshBuffer(const int& InIndex);
	UINT GetDrawObjectNumber() const { return DescribeMeshRenderingData.size(); };
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU存放mesh的顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU存放mesh的索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh的顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh的索引缓冲区

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//临时mesh的顶点缓冲区
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//临时mesh的索引缓冲区

	FMeshRenderingData MeshRenderingData;					//渲染模型存放的顶点数据
	vector<FGeometryDescData> DescribeMeshRenderingData;
};

struct FGeometryMap : public IDirectXDeviceInterface_struct
{
public:
	FGeometryMap();
	void BuildMeshDescData(CMesh* InMesh, const FMeshRenderingData* InRenderingData);
	void BuildMeshBuffer();
	void BuildDescriptorHeap();
	void BuildConstantBufferView();
	UINT GetDrawObjectNumber();
private:
	map<int, FGeometry> Geometrys;
	FDirectXDescriptorHeap DescriptorHeap;
	FDirectXConstBufferView ConstantBufferView;
};