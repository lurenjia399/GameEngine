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


static UINT MeshObjectCount = 0;

FGeometryMap::FGeometryMap()
{
	Geometrys.emplace(0, FGeometry());

	TextureShaderResourceView = make_shared<FRenderingTextureResourcesUpdate>();
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

void FGeometryMap::BuildDescriptorHeap()
{
	//+1 摄像机
	DescriptorHeap.BuildDescriptorHeap(GetDrawMeshObjectCount() + GetDrawLightObjectCount() + GetDrawTextureObjectCount() + 1);
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
			TextureShaderResourceView->LoadTexture(TexturePath);
		}
		
	}
}

void FGeometryMap::BuildMeshConstantBufferView()
{
	MeshConstantBufferView.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectCount());
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	MeshConstantBufferView.BuildConstantBuffer(Handle, GetDrawMeshObjectCount(), 0);
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
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	LightConstantBufferView.BuildConstantBuffer(Handle, GetDrawLightObjectCount(), GetDrawMeshObjectCount());
}

void FGeometryMap::BuildViewportConstantBufferView()
{
	ViewportConstantBufferView.CreateConstant(sizeof(FViewportTransformation), 1);
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	ViewportConstantBufferView.BuildConstantBuffer(Handle, 1, GetDrawMeshObjectCount() + GetDrawLightObjectCount());
}

void FGeometryMap::BuildTextureShaderResource()
{
	TextureShaderResourceView->BuildTextureShaderResource(DescriptorHeap.GetHeap(), GetDrawMeshObjectCount()  + GetDrawLightObjectCount() + 1);
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

void FGeometryMap::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	UpdateMaterialShaderResourceView(DeltaTime, ViewportInfo);

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
	

	//viewport常量缓冲区传入摄像机变换矩阵和透视投影矩阵
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;//切记需要转置，，，主列的矩阵无法乘主行的矩阵
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, ViewProjection);//注意shader读取constBuffer中数据是按照列读取的
	//获取场景中摄像机位置
	ViewportTransformation.cameraPosition = ViewportInfo.cameraPosition;
	//Engine_Log("cameraPisition [x] = %f, [y] = %f, [z] = %f", ViewportInfo.cameraPosition.x, ViewportInfo.cameraPosition.y, ViewportInfo.cameraPosition.z);
	
	ViewportConstantBufferView.Update(0, &ViewportTransformation);
}

void FGeometryMap::UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo)
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
		MaterialTransformation.TextureMapIndex = TextureShaderResourceView->GetTextureIndex(InMaterial->GetMaterialTextureMapKey());
		MaterialTransformation.NormalMapIndex = TextureShaderResourceView->GetTextureIndex(InMaterial->GetMaterialNormalMapKey());
		MaterialTransformation.SpecularMapIndex = TextureShaderResourceView->GetTextureIndex(InMaterial->GetMaterialSpecularMapKey());
		XMFLOAT4X4 materialTransform = InMaterial->GetMaterialTransform();
		XMMATRIX MaterialTransform = XMLoadFloat4x4(&materialTransform);
		XMStoreFloat4x4(&MaterialTransformation.TransformInformation, XMMatrixTranspose(MaterialTransform));
		MaterialConstantBufferView.Update(InMaterial->GetMaterialTextureMapIndex(), &MaterialTransformation);
	}
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	DrawViewport(DeltaTime);
	DrawLight(DeltaTime);
	DrawTexture(DeltaTime);
	DrawMaterial(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(GetDrawMeshObjectCount() + GetDrawLightObjectCount(), HandleSize);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, Handle);
}

void FGeometryMap::DrawTexture(float DeltaTime)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(GetDrawMeshObjectCount() + GetDrawLightObjectCount() + 1, HandleSize);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, Handle);
}

FDirectXDescriptorHeap* FGeometryMap::GetDescriptorHeap()
{
	return &DescriptorHeap;
}

void FGeometryMap::DrawMaterial(float DeltaTime)
{
	GetGraphicsCommandList()->SetGraphicsRootShaderResourceView(4, MaterialConstantBufferView.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawLight(float DeltaTime)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(GetDrawMeshObjectCount(), HandleSize);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, Handle);//根参数中的索引
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
