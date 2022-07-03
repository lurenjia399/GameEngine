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
	void BuildMeshDescData(CMeshComponent* InMesh, const FMeshRenderingData& MeshRenderData, const size_t& HashKey);
	void BuildMeshBuffer(const int& InIndex);
	UINT GetDrawMeshObjectCount() const;
	UINT GetDrawMaterialObjectCount() const;
	bool FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutGeometryDescData, int InRenderingLayer = -1);
	void DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InGeometryDescData);
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
	//vector<FGeometryDescData> DescribeMeshRenderingData;	//����Getometry������
};

class FRenderingTextureResourcesUpdate;
struct FGeometryMap : public IDirectXDeviceInterface_struct
{
public:
	

	FGeometryMap();
	void BuildMeshDescData(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey);
	void BuildMeshBuffer();
	void BuildDescriptorHeap();

	void LoadTexture();

	void BuildMeshConstantBufferView();
	void BuildViewportConstantBufferView();
	void BuildMaterialShaderResourseView();
	void BuildLightConstantBufferView();
	void BuildTextureShaderResource();

	UINT GetDrawMeshObjectCount();
	UINT GetDrawMaterialObjectCount();
	UINT GetDrawLightObjectCount();
	UINT GetDrawTextureObjectCount();

	bool FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutMeshRenderingData, int InRenderingLayer = -1);
	void DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InMeshRenderingData);
	void UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo);
	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);
public:
	void DrawMesh(float DeltaTime);
	void DrawMaterial(float DeltaTime);
	void DrawLight(float DeltaTime);
	void DrawViewport(float DeltaTime);
	void DrawTexture(float DeltaTime);
private:
	map<int, FGeometry> Geometrys;
	FDirectXDescriptorHeap DescriptorHeap;

	FDirectXConstBufferView MeshConstantBufferView;
	FDirectXConstBufferView ViewportConstantBufferView;
	FDirectXConstBufferView MaterialConstantBufferView;
	FDirectXConstBufferView LightConstantBufferView;

	shared_ptr<FRenderingTextureResourcesUpdate> TextureShaderResourceView;
	std::vector<CMaterial*> Materials;
};