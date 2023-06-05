#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "GeometryDescData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/DirectXConstBufferView.h"
#include "../../../../../Component/Sky/FogComponent.h"


struct FGeometry : public IDirectXDeviceInterface_struct
{
	friend struct FGeometryMap;
public:
	bool isExitDescribeMeshRenderingData(CMeshComponent* InKey);
	void BuildMeshDescData(CMeshComponent* InMesh, const FMeshRenderingData& MeshRenderData, const size_t& HashKey, const int& key);
	void BuildMeshBuffer(const int& InIndex);
	UINT GetDrawMeshObjectCount() const;
	UINT GetDrawMaterialObjectCount() const;
	bool FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutGeometryDescData, int InRenderingLayer = -1);
	void DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InGeometryDescData, const int& key);
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
private:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU存放mesh的顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU存放mesh的索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh的顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh的索引缓冲区

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//临时mesh的顶点缓冲区
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//临时mesh的索引缓冲区

	FMeshRenderingData MeshRenderingData;					//渲染模型存放的顶点数据
};


class FRenderingTextureResourcesUpdate;
struct FGeometryMap : public IDirectXDeviceInterface_struct
{
	friend class FRenderingLayer;
public:

	FGeometryMap();
	void BuildMeshDescData(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey);
	void BuildMeshBuffer();
	void BuildDescriptorHeap();

	void LoadTexture();
	int GetTextureIndex(const string& Key);

	void BuildMeshConstantBufferView();
	void BuildViewportConstantBufferView();
	void BuildMaterialShaderResourseView();
	void BuildLightConstantBufferView();
	void BuildTextureShaderResource();
	void BuildFogConstantBufferView();

	UINT GetDrawMeshObjectCount();
	UINT GetDrawMaterialObjectCount();
	UINT GetDrawLightObjectCount();
	UINT GetDrawTextureObjectCount();
	UINT GetDrawCubeMapCount();

	void UpdateLightConstantBufferView(float DeltaTime);
	void UpdateMaterialShaderResourceView(float DeltaTime);
	void UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo, UINT InCBVOffset);
	void UpdateFogConstantBufferView(float DeltaTime);

	bool FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutMeshRenderingData, int InRenderingLayer = -1);
	void DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InMeshRenderingData);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);
public:
	void DrawFog(float DeltaTime);
	void DrawMaterial(float DeltaTime);
	void DrawLight(float DeltaTime);
	void DrawViewport(float DeltaTime);
	void DrawTexture(float DeltaTime);
public:
	bool IsStartUpFog();
public:
	FDirectXDescriptorHeap* GetDescriptorHeap();
private:
	map<int, FGeometry> Geometrys;
	FDirectXDescriptorHeap DescriptorHeap;

	FDirectXConstBufferView MeshConstantBufferView;
	FDirectXConstBufferView ViewportConstantBufferView;
	FDirectXConstBufferView MaterialConstantBufferView;
	FDirectXConstBufferView LightConstantBufferView;
	FDirectXConstBufferView FogConstantBufferView;

	shared_ptr<FRenderingTextureResourcesUpdate> TextureShaderResourceView;
	shared_ptr<FRenderingTextureResourcesUpdate> CubeMapResourceView;
	std::vector<CMaterial*> Materials;
};