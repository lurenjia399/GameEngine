#include "RenderLayerManage.h"
#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/TransparentRenderLayer.h"

FRenderLayerManage* FRenderLayerManage::RenderLayerManage = nullptr;
std::vector<std::shared_ptr<FRenderingLayer>> FRenderLayerManage::RenderingLayers;

FRenderLayerManage::~FRenderLayerManage()
{
	RenderLayerManage = nullptr;
	RenderingLayers.clear();
}

FRenderLayerManage::FRenderLayerManage()
{
	RenderingLayers.clear();

	//在初始化renderlayer的时候，首先默认添加一些层级
	CreateRenderLayer<FAlphaTestRenderLayer>();
	CreateRenderLayer<FOpaqueRenderLayer>();
	CreateRenderLayer<FTransparentRenderLayer>();
}

std::shared_ptr<FRenderingLayer> FRenderLayerManage::FindRenderingLayerByInt(int InRenderLayer)
{
	for (auto& tem : RenderingLayers)
	{
		if (InRenderLayer == tem->GetRenderLayerType())
		{
			return tem;
		}
	}
	Engine_Log_Error("No find Render Layer !!!");
	assert(0);
	return nullptr;
}

void FRenderLayerManage::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	for (auto& tem : RenderingLayers)
	{
		tem->Init(InGeometryMap, InDirectXPiepelineState);
	}
}

void FRenderLayerManage::BuildShader()
{
	for (auto& tem : RenderingLayers)
	{
		tem->BuildShader();
	}
}

void FRenderLayerManage::sort()
{
	auto func = [](const std::shared_ptr<FRenderingLayer>& l, const std::shared_ptr<FRenderingLayer>& r) -> bool
	{
		return l->GetPriority() < r->GetPriority();
	};
	std::sort(RenderingLayers.begin(), RenderingLayers.end(), func);
}

void FRenderLayerManage::PreDraw(float DeltaTime)
{
	for (auto& tem : RenderingLayers)
	{
		tem->PreDraw(DeltaTime);
	}
}

void FRenderLayerManage::Draw(float DeltaTime)
{
	for (auto& tem : RenderingLayers)
	{
		tem->Draw(DeltaTime);
	}
}

void FRenderLayerManage::PostDraw(float DeltaTime)
{
	for (auto& tem : RenderingLayers)
	{
		tem->PostDraw(DeltaTime);
	}
}

void FRenderLayerManage::UpdateObjectConstantBuffer()
{
	for (auto& tem : RenderingLayers)
	{
		tem->UpdateObjectConstantBuffer();
	}
}

void FRenderLayerManage::BuildPSO()
{
	for (auto& tem : RenderingLayers)
	{
		tem->BuildPSO();
	}
}


