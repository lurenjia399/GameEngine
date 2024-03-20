#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "GeometryDescData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/DirectXConstBufferView.h"
#include "../../../../../Component/Sky/FogComponent.h"
#include "../DynamicMap/ShadowMap/DynamicShadowMap.h"


struct FGeometry : public IDirectXDeviceInterface_struct, public std::enable_shared_from_this<FGeometry>
{
	friend struct FGeometryMap;
public:
	bool isExitDescribeMeshRenderingData(std::shared_ptr<CMeshComponent> InKey);
	void BuildMeshDescData(std::shared_ptr<CMeshComponent> InMesh, const FMeshRenderingData& MeshRenderData, const size_t& HashKey, const int& key);
	void BuildMeshBuffer(const int& InIndex);
	UINT GetDrawMeshObjectCount() const;
	UINT GetDrawMaterialObjectCount() const;
	bool FindMeshRenderingDataByHash(const size_t& InHashKey, std::weak_ptr<FGeometryDescData>& OutGeometryDescData, int InRenderingLayer = -1);
	void DuplicateMeshRenderingData(std::shared_ptr<CMeshComponent> InMesh, std::weak_ptr<FGeometryDescData>& InGeometryDescData_weak, const int& key);
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
private:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU存放mesh的顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU存放mesh的索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh的顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh的索引缓冲区

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//临时mesh的顶点缓冲区
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//临时mesh的索引缓冲区

	FMeshRenderingData MeshRenderingData;					//存放了渲染所有mesh的渲染数据
	
	// 唯一渲染池，里面存放一份mesh的引用，不包含重复的
	static map<size_t, std::shared_ptr<FGeometryDescData>> NoRepeatMeshRenderingDataPool;
public:
	// 场景中所有mesh渲染数据的池子，里面可以有重复的
	static vector<std::shared_ptr<FGeometryDescData>> MeshRenderingDataPool;
	static std::map<string, int> RenderingDataIndices;				// key是component的guid,value是在MeshRenderingDataPool数组中的索引
};


class FRenderingTextureResourcesUpdate;
struct FGeometryMap : public IDirectXDeviceInterface_struct
{
	friend class FRenderingLayer;
	friend class FDynamicCubeMap;
	friend class FCubeMapRenderTarget;
	friend class FDynamicShadowMap;
public:

	FGeometryMap();
	void BuildMeshDescData(std::shared_ptr<CMeshComponent> InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey);
	void BuildMeshBuffer();
	void BuildDynamicReflectionMesh();
	void BuildDescriptorHeap();

	void LoadTexture();
	int GetTextureIndex(const string& Key);

	void BuildMeshConstantBufferView();
	void BuildViewportConstantBufferView(UINT InViewportOffset = 0.f);
	void BuildMaterialShaderResourseView();
	void BuildLightConstantBufferView();
	void BuildTextureShaderResource();
	void BuildFogConstantBufferView();

	UINT GetDrawMeshObjectCount();
	UINT GetDrawMaterialObjectCount();
	UINT GetDrawLightObjectCount();
	UINT GetDrawTextureObjectCount();
	UINT GetDrawCubeMapCount();
	UINT GetDynamicReflectionViewportNum();

	void UpdateLightConstantBufferView(float DeltaTime);
	void UpdateMaterialShaderResourceView(float DeltaTime);
	void UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo, UINT InCBVOffset);
	void UpdateFogConstantBufferView(float DeltaTime);
	void UpdateShadowMapShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo);

	bool FindMeshRenderingDataByHash(const size_t& InHashKey, std::weak_ptr<FGeometryDescData>& OutMeshRenderingData, int InRenderingLayer = -1);
	void DuplicateMeshRenderingData(std::shared_ptr<CMeshComponent> InMesh, std::weak_ptr<FGeometryDescData>& InMeshRenderingData);

	void InitDynamicShadowMap(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState);
	void BuildShadowMap();

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);
public:
	void DrawFog(float DeltaTime);
	void DrawMaterial(float DeltaTime);
	void DrawLight(float DeltaTime);
	void DrawViewport(float DeltaTime);
	void Draw2DTexture(float DeltaTime);
	void DrawCubeMapTexture(float DeltaTime);
	void DrawShadowMapTexture(float DeltaTime);
public:
	bool IsStartUpFog();
public:
	//FDirectXDescriptorHeap* GetDescriptorHeap();
private:
	map<int, FGeometry> Geometrys;
	//FDirectXDescriptorHeap DescriptorHeap;// CBV_SRV_UAV这种描述符堆

	FDirectXConstBufferView MeshConstantBufferView;
	FDirectXConstBufferView ViewportConstantBufferView;
	FDirectXConstBufferView MaterialConstantBufferView;
	FDirectXConstBufferView LightConstantBufferView;
	FDirectXConstBufferView FogConstantBufferView;

	shared_ptr<FRenderingTextureResourcesUpdate> TextureShaderResourceView;
	shared_ptr<FRenderingTextureResourcesUpdate> CubeMapResourceView;
	std::vector<shared_ptr<CMaterial>> Materials;

	std::vector<CMeshComponent*> DynamicReflectionMeshComponents;

	FDynamicShadowMap DynamicShadowMap;
};