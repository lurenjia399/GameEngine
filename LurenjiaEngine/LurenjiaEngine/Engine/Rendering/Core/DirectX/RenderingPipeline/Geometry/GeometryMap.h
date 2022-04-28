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
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU���mesh�Ķ��㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU���mesh������������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh������������

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//��ʱmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//��ʱmesh������������

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