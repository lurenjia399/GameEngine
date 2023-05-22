#include "RenderingPipeline.h"

enum class EPiepelineStateType;

FRenderingPipeline::FRenderingPipeline()
{
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHashKey, FGeometryDescData& OutGeometryDescData, int InRenderingLayer)
{
	return GeometryMap.FindMeshRenderingDataByHash(InHashKey, OutGeometryDescData, InRenderingLayer);
}

void FRenderingPipeline::DuplicateMeshRenderingData(CMeshComponent* InMesh, FGeometryDescData& InGeometryDescData)
{
	GeometryMap.DuplicateMeshRenderingData(InMesh, InGeometryDescData);
}

void FRenderingPipeline::BuildMeshComponent(CMeshComponent* InMesh, const FMeshRenderingData& InRenderingData, const size_t& HashKey)
{
	GeometryMap.BuildMeshDescData(InMesh, InRenderingData, HashKey);
}

void FRenderingPipeline::BuildPipeline()
{
	DirectXPiepelineState.ResetGPSDesc();

	//初始化渲染层级
	FRenderLayerManage::GetRenderLayerManage()->Init(&GeometryMap, &DirectXPiepelineState);
	//对渲染层级进行排序
	FRenderLayerManage::GetRenderLayerManage()->sort();

	//加载贴图资源
	GeometryMap.LoadTexture();
	//绑定根签名
	if (!DirectXRootSignature.Build(GeometryMap.GetDrawTextureObjectCount()))
	{
		Engine_Log_Error("构建根签名失败");
	}
	DirectXPiepelineState.BindRootSignature(DirectXRootSignature.GetRootSignature());

	//创建模型资源（顶点和索引）缓冲区
	GeometryMap.BuildMeshBuffer();

	//构建雾的常量缓冲区
	GeometryMap.BuildFogConstantBufferView();
	//构建模型的常量缓冲区
	GeometryMap.BuildMeshConstantBufferView();
	//构建材质的shader资源缓冲区,收集所有的matrial，并且设置material中的shaderindex，这个不是常量缓冲区，所以没有放在描述堆中
	GeometryMap.BuildMaterialShaderResourseView();
	//构建灯光的常量缓冲区
	GeometryMap.BuildLightConstantBufferView();
	//构建视口的常量缓冲区
	GeometryMap.BuildViewportConstantBufferView();

	//创建描述符堆（用于存放常量缓冲区中的资源）, 这里面全是贴图资源
	GeometryMap.BuildDescriptorHeap();
	//构建贴图的着色器资源
	GeometryMap.BuildTextureShaderResource();

	//构建pso
	FRenderLayerManage::GetRenderLayerManage()->BuildPSO();
}

void FRenderingPipeline::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//更新每个层级中模型的常量缓冲区资源
	FRenderLayerManage::GetRenderLayerManage()->UpdateObjectConstantBuffer();
	//更新材质常量缓冲区
	GeometryMap.UpdateMaterialShaderResourceView(DeltaTime, ViewportInfo);
	//更新灯光的常量缓冲区
	GeometryMap.UpdateLightConstantBufferView(DeltaTime, ViewportInfo);
	//更新视口的常量缓冲区
	GeometryMap.UpdateViewportConstantBufferView(DeltaTime, ViewportInfo);
	//更新雾气的常量缓冲区
	GeometryMap.UpdateFogConstantBufferView(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPiepelineState.PreDraw(DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->PreDraw(DeltaTime);
	
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	GeometryMap.PreDraw(DeltaTime);
	DirectXRootSignature.PreDraw(DeltaTime);
	GeometryMap.Draw(DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->Draw(DeltaTime);//Draw每个渲染层级上的mesh
	DirectXPiepelineState.Draw(DeltaTime);//用做捕获keyboard 4 5
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->PostDraw(DeltaTime);
}
