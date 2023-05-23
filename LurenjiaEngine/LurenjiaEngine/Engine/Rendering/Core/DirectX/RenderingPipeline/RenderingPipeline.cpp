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

	{
		/*
		* 这部分资源是模型的顶点、索引数据啥的
		* 这些数据由cpu计算，然后通过上传堆，把数据放到默认堆里面，给gpu读取
		*/
		//创建模型资源（顶点和索引）缓冲区
		GeometryMap.BuildMeshBuffer();
	}

	{
		/*
		* 这部分资源应该是常量，也就是通过cpu来计算数据，然后通过将数据与上传堆映射，进而等同于放到上传堆中
		* gpu每次渲染的时候读取上传堆中的数据，进而在着色器中计算
		* 每次渲染tick，数据都会由cpu重新计算更新，所以放在上传堆中
		*/
		//构建雾的上传堆（也就是创建上传堆，并将雾的数据和上传堆映射起来，下面这几个同样如此）
		GeometryMap.BuildFogConstantBufferView();
		//构建模型的上传堆
		GeometryMap.BuildMeshConstantBufferView();
		//构建材质的上传堆,收集所有的matrial，并且设置material中的shaderindex，
		GeometryMap.BuildMaterialShaderResourseView();
		//构建灯光的上传堆
		GeometryMap.BuildLightConstantBufferView();
		//构建视口的上传堆
		GeometryMap.BuildViewportConstantBufferView();
	}

	{
		/*
		* 这部分资源（贴图资源）应该是在程序运行当中不会改变，然后就把他们创建成了CreateShaderResourceView
		* 然后放在一起，组成描述符堆
		*/
		//创建描述符堆（用于存放贴图资源）
		GeometryMap.BuildDescriptorHeap();
		//构建贴图的着色器资源，将它们放到上边的描述符堆中的描述符中，也就是当作数组元素
		GeometryMap.BuildTextureShaderResource();
	}

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
