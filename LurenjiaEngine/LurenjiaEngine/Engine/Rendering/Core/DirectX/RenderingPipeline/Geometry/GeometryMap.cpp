#include "GeometryMap.h"
#include "../../../../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Core/Camera.h"
#include "../../../../../Core/World.h"
#include "../../../../../Manage/LightManage.h"
#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
#include "../../../../../Component/Light/ParallelLightComponent.h"
#include "../../../../../Component/Light/PointLightComponent.h"
#include "../../../RenderingTextureResourcesUpdate.h"
#include "../RenderingLayer/RenderLayerManage.h"
#include "../../../../../Component/Sky/Core/FogTransformation.h"
#include "../../../../Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Actor/Sky/Fog.h"


static UINT MeshObjectCount = 0;

FGeometryMap::FGeometryMap()
{
	Geometrys.emplace(0, FGeometry());

	//创建texture的SRV，并设置SRV的尺寸
	TextureShaderResourceView = make_shared<FRenderingTextureResourcesUpdate>();
	TextureShaderResourceView->Set_SRV_ViewDimension(D3D12_SRV_DIMENSION_TEXTURE2D);

	//创建cubemap的SRV，并设置SRV的尺寸
	CubeMapResourceView = make_shared<FRenderingTextureResourcesUpdate>();
	CubeMapResourceView->Set_SRV_ViewDimension(D3D12_SRV_DIMENSION_TEXTURECUBE);
}

void FGeometryMap::BuildMeshDescData(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey)
{
	for (auto& tem : Geometrys)
	{
		tem.second.BuildMeshDescData(InMesh, InRenderingData, HashKey, tem.first);
	}
}

void FGeometryMap::BuildMeshBuffer()
{
	for (pair<const int, FGeometry>& Geometry : Geometrys)
	{
		Geometry.second.BuildMeshBuffer(Geometry.first);
	}
}

void FGeometryMap::BuildDynamicReflectionMesh()
{
	// 这里面要根据是否是反射component来将component添加到这个数组里面
	// 现在这里是从所有的池子里挑出动态反射的component
	// 之后改下吧，在设置子材质的时候，顺便写个static数组啥的？记录下
	for (auto& Tmp : ObjectPool)
	{
		if (CMeshComponent* InMeshComponent = dynamic_cast<CMeshComponent*>(Tmp))
		{
			if (InMeshComponent->GetbIsDynamicReflection())
			{
				DynamicReflectionMeshComponents.emplace_back(InMeshComponent);
			}
		}
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	DescriptorHeap.BuildDescriptorHeap(
		GetDrawTextureObjectCount() //texture2d
		+ GetDrawCubeMapCount() //静态cubemap
		+ 1	// 动态cubemap
		+ 1); // 阴影
}

void FGeometryMap::LoadTexture()
{
	def_c_paths Paths;
	init_def_c_paths(&Paths);
	char RootPath[] = "../LurenjiaEngine/Asset";
	find_files(RootPath, &Paths, true);

	for (int i = 0; i < Paths.index; ++i)
	{
		if (find_string(Paths.paths[i], ".dds", 0) != -1)
		{
			normalization_path(Paths.paths[i]);
			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, Paths.paths[i]);
			if (wfind_string(TexturePath, L"_CubeMap") != -1 || wfind_string(TexturePath, L"_cubemap") != -1)
			{
				CubeMapResourceView->LoadTexture(TexturePath);
			}
			else
			{
				TextureShaderResourceView->LoadTexture(TexturePath);
			}
		}
	}
}

int FGeometryMap::GetTextureIndex(const string& Key)
{
	if (TextureShaderResourceView->GetTextureIndex(Key) >= 0)
	{
		return TextureShaderResourceView->GetTextureIndex(Key);
	}
	else
	{
		return CubeMapResourceView->GetTextureIndex(Key);
	}
	
}

void FGeometryMap::BuildMeshConstantBufferView()
{
	MeshConstantBufferView.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectCount());
}

void FGeometryMap::BuildMaterialShaderResourseView()
{
	MaterialConstantBufferView.CreateConstant(sizeof(CMaterialConstantBuffer), GetDrawMaterialObjectCount(), false);
	int ShaderIndex = 0;
	
	for (auto& RenderingLayer : FRenderLayerManage::GetRenderLayerManage()->RenderingLayers)
	{
		for (auto& FGeometryDescData : *RenderingLayer->GetGeometryDescData())
		{
			for (CMaterial* material : *FGeometryDescData.MeshComponet->GetMaterials())
			{
				material->SetMaterialTextureMapIndex(ShaderIndex++);
				Materials.emplace_back(material);
			}
		}
	}
}

void FGeometryMap::BuildLightConstantBufferView()
{
	LightConstantBufferView.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectCount());
	//CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//LightConstantBufferView.BuildConstantBuffer(Handle, GetDrawLightObjectCount(), GetDrawMeshObjectCount());
}

void FGeometryMap::BuildViewportConstantBufferView(UINT InViewportOffset)
{
	ViewportConstantBufferView.CreateConstant(sizeof(FViewportTransformation), 
		1 + //主视口
		GetDynamicReflectionViewportNum() + //这个是动态反射的视口
		1 + // 阴影的视口
		InViewportOffset);
	//CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//ViewportConstantBufferView.BuildConstantBuffer(Handle, 1, GetDrawMeshObjectCount() + GetDrawLightObjectCount());
}

void FGeometryMap::BuildTextureShaderResource()
{
	//构建Texture
	TextureShaderResourceView->BuildTextureShaderResource(DescriptorHeap.GetHeap(), 0);
	//构建cubeMap
	CubeMapResourceView->BuildTextureShaderResource(DescriptorHeap.GetHeap(), GetDrawTextureObjectCount());
}

void FGeometryMap::BuildFogConstantBufferView()
{
	FogConstantBufferView.CreateConstant(sizeof(FFogTransformation), 1);
}

UINT FGeometryMap::GetDrawMeshObjectCount()
{
	return Geometrys[0].GetDrawMeshObjectCount();
}

UINT FGeometryMap::GetDrawMaterialObjectCount()
{
	UINT res = 0;
	for (pair<int, FGeometry> Geometry : Geometrys)
	{
		res += Geometry.second.GetDrawMaterialObjectCount();
	}
	return res;
}

UINT FGeometryMap::GetDrawLightObjectCount()
{
	return 1;
}

UINT FGeometryMap::GetDrawTextureObjectCount()
{
	return TextureShaderResourceView->GetTextureCount();
}

UINT FGeometryMap::GetDrawCubeMapCount()
{
	return CubeMapResourceView->GetTextureCount();
}

UINT FGeometryMap::GetDynamicReflectionViewportNum()
{
	return DynamicReflectionMeshComponents.size() * 6;
}

bool FGeometryMap::FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutGeometryDescData, int InRenderingLayer)
{
	for (pair<int, FGeometry> Geometry : Geometrys)
	{
		if (Geometry.second.FindMeshRenderingDataByHash(InHashKey, OutGeometryDescData, InRenderingLayer))
		{
			return true;
		}
	}
	return false;
}

void FGeometryMap::DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InGeometryDescData)
{
	for (auto& tem : Geometrys)
	{
		tem.second.DuplicateMeshRenderingData(InMesh, InGeometryDescData, tem.first);
	}
}

void FGeometryMap::InitDynamicShadowMap(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	DynamicShadowMap.Init(InGeometryMap, InDirectXPiepelineState);
}

void FGeometryMap::BuildShadowMap()
{
	// 首先创建出阴影需要的摄像机
	DynamicShadowMap.BuildViewport();

	DynamicShadowMap.BuildRenderTargetDescriptor();
}

void FGeometryMap::UpdateLightConstantBufferView(float DeltaTime)
{
	//更新shader中的灯光 常量缓冲区
	FLightConstantBuffer lightTransformation;
	int lightCount = GetLightManage()->Lights.size();
	for (int i = 0; i < lightCount; ++i)
	{
		if (CLightComponent* LightComponent = GetLightManage()->Lights[i])
		{
			lightTransformation.SceneLight[i].LightIntensity = LightComponent->GetLightIntensity();
			lightTransformation.SceneLight[i].Position = LightComponent->GetPosition();
			switch (LightComponent->GetLightType())
			{
			case ELightType::ParallelLight:
				if (CParallelLightComponent* ParallelLightComponent = dynamic_cast<CParallelLightComponent*>(LightComponent))
				{
					lightTransformation.SceneLight[i].LightDirection = ParallelLightComponent->GetForward();
					lightTransformation.SceneLight[i].LightType = (int)ELightType::ParallelLight;


					DynamicShadowMap.BuildViewport(LightComponent->GetPosition(), XMFLOAT3(0.0f, 0.0f, 0.f), 100.0f);

					XMFLOAT4X4 ProjectMatrixfloat = {}, ViewMatrixfloat = {};
					DynamicShadowMap.GetViewportMatrix(ViewMatrixfloat, ProjectMatrixfloat);
					XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ProjectMatrixfloat);
					XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewMatrixfloat);
					XMMATRIX Transform = {
						0.5f,0.0f,0.0f,0.0f,
						0.0f,-0.5f,0.0f,0.0f,
						0.0f,0.0f,1.0f,0.0f,
						0.5f,0.5f,0.0f,1.0f
					};
					// dx的uv坐标在屏幕左上角u朝向右，v朝向下
					//切记需要转置，，，主列的矩阵无法乘主行的矩阵,,,这里得到的因为在片元着色器里面的值，所以也是ndc空间下[-1, 1],需要转成[0, 1]
					// 我看这里应该不对，明天必须解决这个
					XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix * Transform;
					
					XMStoreFloat4x4(&lightTransformation.SceneLight[i].ViewProjectionMatrix, ViewProjection);
				}
				break;
			case ELightType::PointLight:
				if (CPointLightComponent* PointLightComponent = dynamic_cast<CPointLightComponent*>(LightComponent))
				{
					lightTransformation.SceneLight[i].StartAttenuation = PointLightComponent->GetStartAttenuation();
					lightTransformation.SceneLight[i].EndAttenuation = PointLightComponent->GetEndAttenuation();
					lightTransformation.SceneLight[i].LightType = (int)ELightType::PointLight;
				}
				break;
			case ELightType::SpotLight:
				if (CSpotLightComponent* SpotLightComponent = dynamic_cast<CSpotLightComponent*>(LightComponent))
				{
					lightTransformation.SceneLight[i].StartAttenuation = SpotLightComponent->GetStartAttenuation();
					lightTransformation.SceneLight[i].EndAttenuation = SpotLightComponent->GetEndAttenuation();
					lightTransformation.SceneLight[i].LightDirection = SpotLightComponent->GetForward();
					lightTransformation.SceneLight[i].ConicalInnerCorner = math_utils::angle_to_radian(SpotLightComponent->GetConicalInnerCorner());
					lightTransformation.SceneLight[i].ConicalOuterCorner = math_utils::angle_to_radian(SpotLightComponent->GetConicalOuterCorner());
					lightTransformation.SceneLight[i].LightType = (int)ELightType::SpotLight;
				}
				break;
			default:
				break;
			}
		}
	}
	LightConstantBufferView.Update(0, &lightTransformation);
}

void FGeometryMap::UpdateMaterialShaderResourceView(float DeltaTime)
{
	CMaterialConstantBuffer MaterialTransformation;
	for (CMaterial* InMaterial : Materials)
	{
		if (!InMaterial->isDirty()) continue;
		InMaterial->SetDirty(false);

		MaterialTransformation.BaseColor = InMaterial->GetBaseColor();
		MaterialTransformation.SpecularColor = InMaterial->GetSpecularColor();
		MaterialTransformation.MaterialType = static_cast<UINT32>(InMaterial->GetMaterialType());
		MaterialTransformation.Roughness = InMaterial->GetRoughness();
		MaterialTransformation.TextureMapIndex = GetTextureIndex(InMaterial->GetMaterialTextureMapKey());
		MaterialTransformation.NormalMapIndex = GetTextureIndex(InMaterial->GetMaterialNormalMapKey());
		MaterialTransformation.SpecularMapIndex = GetTextureIndex(InMaterial->GetMaterialSpecularMapKey());
		MaterialTransformation.FresnelF0 = InMaterial->GetMaterialFresnelF0();
		XMFLOAT4X4 materialTransform = InMaterial->GetMaterialTransform();
		XMMATRIX MaterialTransform = XMLoadFloat4x4(&materialTransform);
		XMStoreFloat4x4(&MaterialTransformation.TransformInformation, XMMatrixTranspose(MaterialTransform));
		MaterialConstantBufferView.Update(InMaterial->GetMaterialTextureMapIndex(), &MaterialTransformation);
	}
}

void FGeometryMap::UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo, UINT InCBVOffset)
{
	//viewport常量缓冲区传入摄像机变换矩阵和透视投影矩阵
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;//切记需要转置，，，主列的矩阵无法乘主行的矩阵
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, ViewProjection);//注意shader读取constBuffer中数据是按照列读取的
	//获取场景中摄像机位置
	ViewportTransformation.cameraPosition = ViewportInfo.cameraPosition;
	//Engine_Log("cameraPisition [x] = %f, [y] = %f, [z] = %f", ViewportInfo.cameraPosition.x, ViewportInfo.cameraPosition.y, ViewportInfo.cameraPosition.z);

	ViewportConstantBufferView.Update(InCBVOffset, &ViewportTransformation);
}

void FGeometryMap::UpdateFogConstantBufferView(float DeltaTime)
{
	CFogComponent* FogComponent = GetEngine()->GetRenderingEngine()->GetWorld()->GetFog()->GetComponent();
	if (FogComponent != nullptr)
	{
		if (!FogComponent->GetDirtyState()) return;
		FogComponent->SetDirtyState(false);

		FFogTransformation FogTransformation = {};
		FogTransformation.FogColor = FogComponent->GetFogColor();
		FogTransformation.FogStart = FogComponent->GetFogStart();
		FogTransformation.FogRange = FogComponent->GetFogRange();
		FogTransformation.FogHeight = FogComponent->GetFogHeight();
		FogTransformation.FogTransparentCoefficient = FogComponent->GetFogTransparentCoefficient();
		FogConstantBufferView.Update(0, &FogTransformation);
	}

}

void FGeometryMap::UpdateShadowMapShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	DynamicShadowMap.UpdateViewportConstantBufferView(DeltaTime, ViewportInfo);
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	DrawFog(DeltaTime);
	//DrawViewport(DeltaTime);
	DrawLight(DeltaTime);
	Draw2DTexture(DeltaTime);
	DrawCubeMapTexture(DeltaTime);
	DrawMaterial(DeltaTime);

	DynamicShadowMap.Draw(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

FDirectXDescriptorHeap* FGeometryMap::GetDescriptorHeap()
{
	return &DescriptorHeap;
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportConstantBufferView.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawLight(float DeltaTime)
{

	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(2, LightConstantBufferView.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawFog(float DeltaTime)
{
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(3, FogConstantBufferView.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawMaterial(float DeltaTime)
{
	GetGraphicsCommandList()->SetGraphicsRootShaderResourceView(4, MaterialConstantBufferView.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::Draw2DTexture(float DeltaTime)
{
	UINT HandleSize = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, Handle);
}

void FGeometryMap::DrawCubeMapTexture(float DeltaTime)
{
	
	UINT HandleSize = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(GetDrawTextureObjectCount(), HandleSize);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, Handle);
}

void FGeometryMap::DrawShadowMapTexture(float DeltaTime)
{
	DynamicShadowMap.DrawShadowMapTexture(DeltaTime);
}

bool FGeometryMap::IsStartUpFog()
{
	CFogComponent* FogComponent = GetEngine()->GetRenderingEngine()->GetWorld()->GetFog()->GetComponent();
	return FogComponent != nullptr;
}

bool FGeometry::isExitDescribeMeshRenderingData(CMeshComponent* InKey)
{
	//根据meshcomponent所处的层级，添加geometryDescdata
	std::shared_ptr<FRenderingLayer> RenderLayer = FRenderLayerManage::FindRenderingLayerByInt((int)InKey->GetMeshComponentLayerType());
	vector<FGeometryDescData> DescribeMeshRenderingData = *RenderLayer->GetGeometryDescData();

	for (FGeometryDescData& data : DescribeMeshRenderingData)
	{
		if (data.MeshComponet == InKey)
		{
			return true;
		}
	}
	return false;
}

void FGeometry::BuildMeshDescData(CMeshComponent* InMesh, const FMeshRenderingData& MeshRenderData, const size_t& HashKey, const int& key)
{
	if (!isExitDescribeMeshRenderingData(InMesh))
	{
		//根据meshcomponent所处的层级，添加geometryDescdata
		std::shared_ptr<FRenderingLayer> RenderLayer = FRenderLayerManage::FindRenderingLayerByInt((int)InMesh->GetMeshComponentLayerType());
		vector<FGeometryDescData>& DescribeMeshRenderingData = *RenderLayer->GetGeometryDescData();

		DescribeMeshRenderingData.emplace_back(FGeometryDescData());
		FGeometryDescData& GeometryDescData = DescribeMeshRenderingData.back();
		GeometryDescData.MeshComponet = InMesh;
		GeometryDescData.MeshHash = HashKey;
		GeometryDescData.MeshObjectOffset = MeshObjectCount++;
		GeometryDescData.GeometryKey = key;
		GeometryDescData.IndexSize = MeshRenderData.IndexData.size();
		GeometryDescData.IndexoffsetPosition = MeshRenderingData.IndexData.size();
		GeometryDescData.VertexSize = MeshRenderData.VertexData.size();
		GeometryDescData.VertexoffsetPostion = MeshRenderingData.VertexData.size();

		MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshRenderData.IndexData.begin(), MeshRenderData.IndexData.end());
		MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshRenderData.VertexData.begin(), MeshRenderData.VertexData.end());

	}
}

void FGeometry::BuildMeshBuffer(const int& InIndex)
{
	//在cpu中创建资源缓冲区
	ANALYSIS_HRESULT(D3DCreateBlob(MeshRenderingData.GetVertexSizeInBytes(), &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), MeshRenderingData.GetVertexSizeInBytes());
	ANALYSIS_HRESULT(D3DCreateBlob(MeshRenderingData.GetIndexSizeInBytes(), &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), MeshRenderingData.GetIndexSizeInBytes());

	//在gpu中创建资源缓冲区
	ConstructBuffer::FConstructBuffer ConstructBuffer{};
	GPUVertexBufferPtr = ConstructBuffer.ConstructGPUDefaultBuffer(VertexBufferTempPtr, MeshRenderingData.VertexData.data(), MeshRenderingData.GetVertexSizeInBytes());
	GPUIndexBufferPtr = ConstructBuffer.ConstructGPUDefaultBuffer(IndexBufferTempPtr, MeshRenderingData.IndexData.data(), MeshRenderingData.GetIndexSizeInBytes());
	if (GPUIndexBufferPtr == nullptr || GPUVertexBufferPtr == nullptr)
	{
		Engine_Log_Error("FGeometry::BuildMeshBuffer error");
		return;
	}
}

UINT FGeometry::GetDrawMeshObjectCount() const
{
	return MeshObjectCount;
}

UINT FGeometry::GetDrawMaterialObjectCount() const
{
	UINT res = 0;
	for (auto& RenderLayer : FRenderLayerManage::RenderingLayers)
	{
		vector<FGeometryDescData> DescribeMeshRenderingData = *RenderLayer->GetGeometryDescData();
		for (FGeometryDescData GeometryDesc : DescribeMeshRenderingData)
		{
			res += GeometryDesc.MeshComponet->GetMaterialsCount();
		}
	}
	
	return res;
}

bool FGeometry::FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutGeometryDescData, int InRenderingLayer)
{
	auto FindMeshRenderingDataByHashSub = [&](const shared_ptr<FRenderingLayer>& InRenderintLayers) -> bool
	{
		vector<FGeometryDescData> DescribeMeshRenderingData = *InRenderintLayers->GetGeometryDescData();
		for (FGeometryDescData& tem : DescribeMeshRenderingData)
		{
			if (tem.MeshHash == InHashKey)
			{
				OutGeometryDescData = tem;
				return true;
			}
		}
		return false;
	};

	if (InRenderingLayer == -1)
	{
		for (auto& RenderLayer : FRenderLayerManage::RenderingLayers)
		{
			if (FindMeshRenderingDataByHashSub(RenderLayer))
			{
				return true;
			}
		}
		return false;
	}
	else {
		std::shared_ptr<FRenderingLayer> RenderLayer = FRenderLayerManage::FindRenderingLayerByInt(InRenderingLayer);
		return FindMeshRenderingDataByHashSub(RenderLayer);
	}
}

void FGeometry::DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InGeometryDescData, const int& key)
{
	if (!isExitDescribeMeshRenderingData(InMesh))
	{
		std::shared_ptr<FRenderingLayer> RenderLayer = FRenderLayerManage::FindRenderingLayerByInt((int)InMesh->GetMeshComponentLayerType());
		vector<FGeometryDescData>& DescribeMeshRenderingData = *RenderLayer->GetGeometryDescData();

		DescribeMeshRenderingData.emplace_back(InGeometryDescData);//这里应该不对，有待考证
		FGeometryDescData& GeometryDescData = DescribeMeshRenderingData.back();
		GeometryDescData.MeshComponet = InMesh;
		GeometryDescData.MeshObjectOffset = MeshObjectCount++;
		GeometryDescData.GeometryKey = key;
	}
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV = {};
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	VBV.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	VBV.StrideInBytes = sizeof(FVertex);
	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV = {};
	IBV.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();
	IBV.Format = DXGI_FORMAT_R16_UINT;
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();

	return IBV;
}
