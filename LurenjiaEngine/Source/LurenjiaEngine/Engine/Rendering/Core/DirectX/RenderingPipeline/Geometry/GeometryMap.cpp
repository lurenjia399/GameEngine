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

map<size_t, std::shared_ptr<FGeometryDescData>> FGeometry::NoRepeatMeshRenderingDataPool;

vector<std::shared_ptr<FGeometryDescData>> FGeometry::MeshRenderingDataPool;
std::map<string, int> FGeometry::RenderingDataIndices;

FGeometryMap::FGeometryMap()
{
	Geometrys.emplace(0, FGeometry());

	//����texture��SRV��������SRV�ĳߴ�
	TextureShaderResourceView = make_shared<FRenderingTextureResourcesUpdate>();
	TextureShaderResourceView->Set_SRV_ViewDimension(D3D12_SRV_DIMENSION_TEXTURE2D);

	//����cubemap��SRV��������SRV�ĳߴ�
	CubeMapResourceView = make_shared<FRenderingTextureResourcesUpdate>();
	CubeMapResourceView->Set_SRV_ViewDimension(D3D12_SRV_DIMENSION_TEXTURECUBE);
}

void FGeometryMap::BuildMeshDescData(std::shared_ptr<CMeshComponent> InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey)
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
	// ������Ҫ�����Ƿ��Ƿ���component����component��ӵ������������
	// ���������Ǵ����еĳ�����������̬�����component
	// ֮����°ɣ��������Ӳ��ʵ�ʱ��˳��д��static����ɶ�ģ���¼��
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
	GetCBV_SRV_UAVHeap()->BuildRenderingDescriptorHeap(
		GetDrawTextureObjectCount() //texture2d 6��
		+ GetDrawCubeMapCount() //��̬cubemap 1��
		+ 1	// ��̬cubemap
		+ 1 // ��Ӱ
#if (OPENCOMPUTEPIPELINE == 1)
		+ 2 // computeShader pIBlurMap0 ��SRV��UAV
		+ 2 // computeShader pIBlurMap1 ��SRV��UAV
#endif
		+ 1 // editor ui
	);
}

void FGeometryMap::LoadTexture()
{
	def_c_paths Paths;
	init_def_c_paths(&Paths);
	string ContentPath = FEnginePathHelper::GetEngineContentPath();
	find_files(ContentPath.c_str(), &Paths, true, true);

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
		for (auto& FGeometryDescData_weak : *RenderingLayer->GetGeometryDescData())
		{
			if (FGeometryDescData_weak.expired())
			{
				Engine_Log_Error("FGeometryMap::BuildMaterialShaderResourseView FGeometryDescData_weak is nillptr");
				continue;
			}

			std::shared_ptr<FGeometryDescData> FGeometryDescData = FGeometryDescData_weak.lock();

			for (shared_ptr<CMaterial> material : FGeometryDescData->MeshComponet->GetMaterials())
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
		1 + //���ӿ�
		GetDynamicReflectionViewportNum() + //����Ƕ�̬������ӿ�
		1 + // ��Ӱ���ӿ�
		InViewportOffset);
	//CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//ViewportConstantBufferView.BuildConstantBuffer(Handle, 1, GetDrawMeshObjectCount() + GetDrawLightObjectCount());
}

void FGeometryMap::BuildTextureShaderResource()
{
	//����Texture
	TextureShaderResourceView->BuildTextureShaderResource(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), 0);
	//����cubeMap
	CubeMapResourceView->BuildTextureShaderResource(GetCBV_SRV_UAVHeap()->GetRenderingHeap(), GetDrawTextureObjectCount());
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
	return static_cast<UINT>(DynamicReflectionMeshComponents.size()) * 6;
}

bool FGeometryMap::FindMeshRenderingDataByHash(const size_t& InHashKey, std::weak_ptr<FGeometryDescData>& OutGeometryDescData, int InRenderingLayer)
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

void FGeometryMap::DuplicateMeshRenderingData(std::shared_ptr<CMeshComponent> InMesh, std::weak_ptr<FGeometryDescData>& InGeometryDescData)
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
	// ���ȴ�������Ӱ��Ҫ�������
	DynamicShadowMap.BuildViewport();

	DynamicShadowMap.BuildRenderTargetDescriptor();
}

void FGeometryMap::UpdateLightConstantBufferView(float DeltaTime)
{
	//����shader�еĵƹ� ����������
	FLightConstantBuffer lightTransformation;
	int lightCount = static_cast<int>(GetLightManage()->Lights.size());
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


					
					DynamicShadowMap.BuildViewport(LightComponent->GetForward(), XMFLOAT3(0.0f, 0.0f, 0.f), 100.0f);

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
					// dx��uv��������Ļ���Ͻ�u�����ң�v������
					//�м���Ҫת�ã��������еľ����޷������еľ���,,,����õ�����Ϊ��ƬԪ��ɫ�������ֵ������Ҳ��ndc�ռ���[-1, 1],��Ҫת��[0, 1]
					XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;
					
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
	for (shared_ptr<CMaterial> InMaterial : Materials)
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
	//viewport��������������������任�����͸��ͶӰ����
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;//�м���Ҫת�ã��������еľ����޷������еľ���
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, ViewProjection);//ע��shader��ȡconstBuffer�������ǰ����ж�ȡ��
	//��ȡ�����������λ��
	ViewportTransformation.cameraPosition = ViewportInfo.cameraPosition;
	//Engine_Log("cameraPisition [x] = %f, [y] = %f, [z] = %f", ViewportInfo.cameraPosition.x, ViewportInfo.cameraPosition.y, ViewportInfo.cameraPosition.z);

	ViewportConstantBufferView.Update(InCBVOffset, &ViewportTransformation);
}

void FGeometryMap::UpdateFogConstantBufferView(float DeltaTime)
{
	if (IsStartUpFog())
	{
		shared_ptr<CFogComponent> FogComponent = GetEngine()->GetRenderingEngine()->GetWorld()->GetFog()->GetComponent();
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
}

void FGeometryMap::UpdateShadowMapShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	DynamicShadowMap.UpdateViewportConstantBufferView(DeltaTime, ViewportInfo);
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	GetCBV_SRV_UAVHeap()->PreDraw(DeltaTime);
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

//FDirectXDescriptorHeap* FGeometryMap::GetDescriptorHeap()
//{
//	return &GetCBV_SRV_UAVHeap();
//}

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
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetCBV_SRV_UAVHeap()->GetRenderingHeap()->GetGPUDescriptorHandleForHeapStart());
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, Handle);
}

void FGeometryMap::DrawCubeMapTexture(float DeltaTime)
{
	
	UINT HandleSize = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetCBV_SRV_UAVHeap()->GetRenderingHeap()->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(GetDrawTextureObjectCount(), HandleSize);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, Handle);
}

void FGeometryMap::DrawShadowMapTexture(float DeltaTime)
{
	DynamicShadowMap.DrawShadowMapTexture(DeltaTime);
}

bool FGeometryMap::IsStartUpFog()
{
	if (shared_ptr<AFog> temp = GetEngine()->GetRenderingEngine()->GetWorld()->GetFog())
	{
		return temp->GetFogIsValid();
	}
	return false;
}

bool FGeometry::isExitDescribeMeshRenderingData(std::shared_ptr<CMeshComponent> InKey)
{
	//����meshcomponent�����Ĳ㼶�����geometryDescdata
	std::shared_ptr<FRenderingLayer> RenderLayer = FRenderLayerManage::FindRenderingLayerByInt((int)InKey->GetMeshComponentLayerType());
	std::vector<std::weak_ptr<FGeometryDescData>> DescribeMeshRenderingData = *RenderLayer->GetGeometryDescData();

	for (std::weak_ptr<FGeometryDescData>& data_weak : DescribeMeshRenderingData)
	{
		if (data_weak.expired())
		{
			Engine_Log_Error("FGeometry::isExitDescribeMeshRenderingData data_weak is nullptr");
			continue;
		}

		auto data = data_weak.lock();

		if (data->MeshComponet == InKey)
		{
			return true;
		}
	}
	return false;
}

// ��RenderingLayer���������Ⱦ����
// ��mesh��Ⱦ���ݱ��浽MeshRenderingData����
void FGeometry::BuildMeshDescData(std::shared_ptr<CMeshComponent> InMesh, const FMeshRenderingData& MeshRenderData, const size_t& HashKey, const int& key)
{
		
	XMVECTOR center_v = {}, extents_v = {};
	{
		// ��AABB��Χ��
		XMFLOAT3 MaxPoint = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		XMFLOAT3 MinPoint = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
		for (const FVertex& vertex : MeshRenderData.VertexData)
		{
			MinPoint.x = min(vertex.Pos.x, MinPoint.x);
			MinPoint.y = min(vertex.Pos.y, MinPoint.y);
			MinPoint.z = min(vertex.Pos.z, MinPoint.z);

			MaxPoint.x = max(vertex.Pos.x, MaxPoint.x);
			MaxPoint.y = max(vertex.Pos.y, MaxPoint.y);
			MaxPoint.z = max(vertex.Pos.z, MaxPoint.z);
		}
		XMFLOAT3 center = {}, extents = {};
		extents.x = (MaxPoint.x - MinPoint.x) * 0.5f;
		extents.y = (MaxPoint.y - MinPoint.y) * 0.5f;
		extents.z = (MaxPoint.z - MinPoint.z) * 0.5f;
		center.x = extents.x + MinPoint.x;
		center.y = extents.y + MinPoint.y;
		center.z = extents.z + MinPoint.z;
		center_v = XMLoadFloat3(&center);
		extents_v = XMLoadFloat3(&extents);
	}
		
	{
		// ��Ⱦ�õ����ݴ洢
		// ����Ⱦ������ӵ��ظ�������
		MeshRenderingDataPool.emplace_back(std::make_shared<FGeometryDescData>());
		auto& PoolRenderData = MeshRenderingDataPool.back();
		PoolRenderData->MeshComponet = InMesh;
		PoolRenderData->MeshHash = HashKey;
		PoolRenderData->MeshObjectOffset = MeshObjectCount++;
		PoolRenderData->GeometryKey = key;
		PoolRenderData->IndexSize = static_cast<UINT>(MeshRenderData.IndexData.size());
		PoolRenderData->IndexoffsetPosition = static_cast<UINT>(MeshRenderingData.IndexData.size());
		PoolRenderData->VertexSize = static_cast<UINT>(MeshRenderData.VertexData.size());
		PoolRenderData->VertexoffsetPostion = static_cast<UINT>(MeshRenderingData.VertexData.size());
		PoolRenderData->MeshRenderingData = &MeshRenderingData;
		XMStoreFloat3(&PoolRenderData->AABB_box.Center, center_v);
		XMStoreFloat3(&PoolRenderData->AABB_box.Extents, extents_v);

		// ����Ⱦ������ӵ��㼶����
		std::shared_ptr<FRenderingLayer> RenderLayer = FRenderLayerManage::FindRenderingLayerByInt((int)InMesh->GetMeshComponentLayerType());
		vector<std::weak_ptr<FGeometryDescData>>& DescribeMeshRenderingDatas = *RenderLayer->GetGeometryDescData();
		DescribeMeshRenderingDatas.emplace_back(PoolRenderData);

		RenderingDataIndices.insert(std::make_pair<string, int>(InMesh->GetGuid(), (int)MeshRenderingDataPool.size() - 1));
	}

	{
		// Ψһ���ݴ洢
		NoRepeatMeshRenderingDataPool.insert(std::make_pair(HashKey, std::make_shared<FGeometryDescData>()));
		NoRepeatMeshRenderingDataPool[HashKey]->MeshComponet = InMesh;
		NoRepeatMeshRenderingDataPool[HashKey]->MeshHash = HashKey;
		NoRepeatMeshRenderingDataPool[HashKey]->MeshObjectOffset = MeshObjectCount;//���￴��ȥ��Ӧ��++�˰�
		NoRepeatMeshRenderingDataPool[HashKey]->GeometryKey = key;
		NoRepeatMeshRenderingDataPool[HashKey]->IndexSize = static_cast<UINT>(MeshRenderData.IndexData.size());
		NoRepeatMeshRenderingDataPool[HashKey]->IndexoffsetPosition = static_cast<UINT>(MeshRenderingData.IndexData.size());
		NoRepeatMeshRenderingDataPool[HashKey]->VertexSize = static_cast<UINT>(MeshRenderData.VertexData.size());
		NoRepeatMeshRenderingDataPool[HashKey]->VertexoffsetPostion = static_cast<UINT>(MeshRenderingData.VertexData.size());
		NoRepeatMeshRenderingDataPool[HashKey]->MeshRenderingData = &MeshRenderingData;
		XMStoreFloat3(&NoRepeatMeshRenderingDataPool[HashKey]->AABB_box.Center, center_v);
		XMStoreFloat3(&NoRepeatMeshRenderingDataPool[HashKey]->AABB_box.Extents, extents_v);
	}

	{
		MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshRenderData.IndexData.begin(), MeshRenderData.IndexData.end());
		MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshRenderData.VertexData.begin(), MeshRenderData.VertexData.end());
	}

}

void FGeometry::BuildMeshBuffer(const int& InIndex)
{
	//��cpu�д�����Դ������
	ANALYSIS_HRESULT(D3DCreateBlob(MeshRenderingData.GetVertexSizeInBytes(), &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), MeshRenderingData.GetVertexSizeInBytes());
	ANALYSIS_HRESULT(D3DCreateBlob(MeshRenderingData.GetIndexSizeInBytes(), &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), MeshRenderingData.GetIndexSizeInBytes());

	//��gpu�д�����Դ������
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
		vector<std::weak_ptr<FGeometryDescData>> DescribeMeshRenderingData = *RenderLayer->GetGeometryDescData();
		for (auto& GeometryDesc_weak : DescribeMeshRenderingData)
		{
			if (GeometryDesc_weak.expired())
			{
				Engine_Log_Error("FGeometry::GetDrawMaterialObjectCount GeometryDesc_weak is nullptr");
				continue;
			}

			auto GeometryDesc = GeometryDesc_weak.lock();
			res += GeometryDesc->MeshComponet->GetMaterialsCount();
		}
	}
	
	return res;
}


bool FGeometry::FindMeshRenderingDataByHash(const size_t& InHashKey, std::weak_ptr<FGeometryDescData>& OutGeometryDescData, int InRenderingLayer)
{

	if (NoRepeatMeshRenderingDataPool.size() <= 0)
	{
		return false;
	}

	map<size_t, std::shared_ptr<FGeometryDescData>>::iterator data = NoRepeatMeshRenderingDataPool.find(InHashKey);
	if (data != NoRepeatMeshRenderingDataPool.end())
	{
		OutGeometryDescData = data->second;
		return true;
	}

	return false;
}

void FGeometry::DuplicateMeshRenderingData(std::shared_ptr<CMeshComponent> InMesh, std::weak_ptr<FGeometryDescData>& InGeometryDescData_weak, const int& key)
{
	if (InGeometryDescData_weak.expired())
	{
		Engine_Log_Error("FGeometry::DuplicateMeshRenderingData InGeometryDescData_weak is nullptr");
		return;
	}
	std::shared_ptr<FGeometryDescData> InGeometryDescData = InGeometryDescData_weak.lock();

	MeshRenderingDataPool.emplace_back(std::make_shared<FGeometryDescData>());
	std::shared_ptr<FGeometryDescData>& PoolRenderData = MeshRenderingDataPool.back();
	PoolRenderData->MeshComponet = InMesh;
	PoolRenderData->MeshHash = InGeometryDescData->MeshHash;
	PoolRenderData->MeshObjectOffset = MeshObjectCount++;
	PoolRenderData->GeometryKey = key;
	PoolRenderData->IndexSize = InGeometryDescData->IndexSize;
	PoolRenderData->IndexoffsetPosition = InGeometryDescData->IndexoffsetPosition;
	PoolRenderData->VertexSize = InGeometryDescData->VertexSize;
	PoolRenderData->VertexoffsetPostion = InGeometryDescData->VertexoffsetPostion;
	PoolRenderData->MeshRenderingData = &MeshRenderingData;
	PoolRenderData->AABB_box.Center = InGeometryDescData->AABB_box.Center;
	PoolRenderData->AABB_box.Extents = InGeometryDescData->AABB_box.Extents;


	std::shared_ptr<FRenderingLayer> RenderLayer = FRenderLayerManage::FindRenderingLayerByInt((int)InMesh->GetMeshComponentLayerType());
	vector<std::weak_ptr<FGeometryDescData>>& DescribeMeshRenderingData = *RenderLayer->GetGeometryDescData();
	DescribeMeshRenderingData.emplace_back(PoolRenderData);

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
