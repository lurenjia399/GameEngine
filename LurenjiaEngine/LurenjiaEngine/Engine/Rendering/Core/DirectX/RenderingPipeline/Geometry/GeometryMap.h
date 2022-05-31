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
	//friend void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	friend struct FGeometryMap;
public:
	bool isExitDescribeMeshRenderingData(CMeshComponent* InKey);
	void BuildMeshDescData(CMeshComponent* InMesh, const FMeshRenderingData& MeshRenderData);
	void BuildMeshBuffer(const int& InIndex);
	UINT GetDrawMeshObjectCount() const;
	UINT GetDrawMaterialObjectCount() const;
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
private:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU���mesh�Ķ��㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU���mesh������������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh������������

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//��ʱmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//��ʱmesh������������

	FMeshRenderingData MeshRenderingData;					//��Ⱦģ�ʹ�ŵĶ�������
	vector<FGeometryDescData> DescribeMeshRenderingData;	//����Getometry������
};

struct FGeometryMap : public IDirectXDeviceInterface_struct
{
public:
	FGeometryMap();
	void BuildMeshDescData(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData);
	void BuildMeshBuffer();
	void BuildDescriptorHeap();

	void BuildMeshConstantBufferView();
	void BuildViewportConstantBufferView();
	void BuildMaterialConstantBufferView();
	void BuildLightConstantBufferView();

	UINT GetDrawMeshObjectCount();
	UINT GetDrawMaterialObjectCount();
	UINT GetDrawLightObjectCount();

	void UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo);
	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);
public:
	void DrawMesh(float DeltaTime);
	void DrawLight(float DeltaTime);
	void DrawViewport(float DeltaTime);
private:
	map<int, FGeometry> Geometrys;
	FDirectXDescriptorHeap DescriptorHeap;

	FDirectXConstBufferView MeshConstantBufferView;
	FDirectXConstBufferView ViewportConstantBufferView;
	FDirectXConstBufferView MaterialConstantBufferView;
	FDirectXConstBufferView LightConstantBufferView;
};