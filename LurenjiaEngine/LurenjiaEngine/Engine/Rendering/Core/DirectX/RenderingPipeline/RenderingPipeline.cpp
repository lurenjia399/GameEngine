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

	{
		//初始化渲染层级
		FRenderLayerManage::GetRenderLayerManage()->Init(&GeometryMap, &DirectXPiepelineState);
		//对渲染层级进行排序
		FRenderLayerManage::GetRenderLayerManage()->sort();
	}

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
		//构建动态反射Mesh
		GeometryMap.BuildDynamicReflectionMesh();
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
		GeometryMap.BuildViewportConstantBufferView(0);
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

	{
		// 构建动态的cubemap
		DynamicCubeMap.Init(&GeometryMap, &DirectXPiepelineState);
		DynamicCubeMap.BuildViewport(XMFLOAT3(0.f, 5.f, 2.f)); // 那个测试球的位置

		//构建深度模板描述符句柄（记住先后顺序，先构建句柄然后在构建描述符）
		DynamicCubeMap.BuildDepthStencilDescriptorHandle();
		//构建深度模板描述符
		DynamicCubeMap.BuildDepthStencilView();

		//构建RTV和SRV
		DynamicCubeMap.BuildRenderTargetDescriptor();
	}

	//构建pso
	FRenderLayerManage::GetRenderLayerManage()->BuildPSO();
}

void FRenderingPipeline::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//更新动态反射视口常量缓冲区
	DynamicCubeMap.UpdateViewportConstantBufferView(DeltaTime, ViewportInfo);
	//更新每个层级中模型的常量缓冲区资源
	FRenderLayerManage::GetRenderLayerManage()->UpdateObjectConstantBuffer();
	//更新材质常量缓冲区
	GeometryMap.UpdateMaterialShaderResourceView(DeltaTime);
	//更新灯光的常量缓冲区
	GeometryMap.UpdateLightConstantBufferView(DeltaTime);
	//更新视口的常量缓冲区
	GeometryMap.UpdateViewportConstantBufferView(DeltaTime, ViewportInfo, 0);
	//更新雾气的常量缓冲区
	GeometryMap.UpdateFogConstantBufferView(DeltaTime);
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	// 向命令列表中添加 设置pso
	DirectXPiepelineState.PreDraw(DeltaTime);

	// 向命令列表中添加 描述符堆，这个堆里存储的各种不变的资源，贴图
	GeometryMap.PreDraw(DeltaTime);
	// 向命令列表中添加 根签名
	DirectXRootSignature.PreDraw(DeltaTime);

	// 这个方法就是绑定跟签名，感觉需要改下名字更好
	GeometryMap.Draw(DeltaTime);

	// 清掉rtv上面的东西
	ClearMainSwapChain();

	// 渲染动态反射
	if (DynamicCubeMap.IsExitDynamicReflectionMesh())
	{
		DynamicCubeMap.PreDraw(DeltaTime);
	}
	

	// 这个里面现在啥都没有
	FRenderLayerManage::GetRenderLayerManage()->PreDraw(DeltaTime);
	
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	
	GeometryMap.DrawViewport(DeltaTime);

	// 这里面全是根据根签名的序号，设置gpu内存地址，也就是告诉着色器相应的资源放在哪个寄存器里面了
	GeometryMap.DrawCubeMapTexture(DeltaTime);

	// Draw每个渲染层级 背景 -> 不透明物体 -> 透明物体
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_BACKGROUND, DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_OPAQUE, DeltaTime);
	FRenderLayerManage::GetRenderLayerManage()->Draw((int)EMeshComponentRenderLayerType::RENDERLAYER_TRANSPARENT, DeltaTime);

	// 切换pso用的，放在这合适么?
	DirectXPiepelineState.Draw(DeltaTime);//用做捕获keyboard 4 5
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	// 目前这里面啥都没有
	GeometryMap.PostDraw(DeltaTime);
	// 目前这里面啥都没有
	FRenderLayerManage::GetRenderLayerManage()->PostDraw(DeltaTime);
}
