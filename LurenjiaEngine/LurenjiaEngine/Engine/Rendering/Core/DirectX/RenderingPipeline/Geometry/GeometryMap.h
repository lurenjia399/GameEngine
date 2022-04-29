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
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU���mesh�Ķ��㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU���mesh������������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh������������

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//��ʱmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//��ʱmesh������������

	FMeshRenderingData MeshRenderingData;					//��Ⱦģ�ʹ�ŵĶ�������
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