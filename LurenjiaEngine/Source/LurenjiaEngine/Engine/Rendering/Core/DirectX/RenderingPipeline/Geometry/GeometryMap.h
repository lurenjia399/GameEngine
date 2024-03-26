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
	ComPtr<ID3DBlob> CPUVertexBufferPtr;					//CPU���mesh�Ķ��㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;						//CPU���mesh������������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;				//GPUmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;				//GPUmesh������������

	ComPtr<ID3D12Resource> VertexBufferTempPtr;				//��ʱmesh�Ķ��㻺����
	ComPtr<ID3D12Resource> IndexBufferTempPtr;				//��ʱmesh������������

	FMeshRenderingData MeshRenderingData;					//�������Ⱦ����mesh����Ⱦ����
	
	// Ψһ��Ⱦ�أ�������һ��mesh�����ã��������ظ���
	static map<size_t, std::shared_ptr<FGeometryDescData>> NoRepeatMeshRenderingDataPool;
public:
	// ����������mesh��Ⱦ���ݵĳ��ӣ�����������ظ���
	static vector<std::shared_ptr<FGeometryDescData>> MeshRenderingDataPool;
	static std::map<string, int> RenderingDataIndices;				// key��component��guid,value����MeshRenderingDataPool�����е�����
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
	//FDirectXDescriptorHeap DescriptorHeap;// CBV_SRV_UAV������������

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