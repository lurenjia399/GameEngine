#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"

struct FGeometry : public IDirectXDeviceInterface_struct
{
public:
	bool isExitRenderingData(CMesh* InKey);
	void BuildMesh(CMesh* InMesh, const FMeshRenderingData* MeshRenderData);
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU存放mesh的顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU存放mesh的索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh的顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh的索引缓冲区

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//临时mesh的顶点缓冲区
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//临时mesh的索引缓冲区

	FMeshRenderingData MeshRenderingData;
	vector<FRenderingData> DescribeMeshRenderingData;
};

struct FGeometryMap : public IDirectXDeviceInterface_struct
{
public:
	FGeometryMap();
	void BuildMesh(CMesh* InMesh, const FMeshRenderingData* InRenderingData);
private:
	map<int, FGeometry> Geometrys;
};