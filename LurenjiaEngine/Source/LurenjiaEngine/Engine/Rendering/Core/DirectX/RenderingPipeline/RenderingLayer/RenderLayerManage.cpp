#include "RenderLayerManage.h"
#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/TransparentRenderLayer.h"
#include "RenderLayer/BackgroundRenderLayer.h"
#include "RenderLayer/OpaqueReflectorRenderLayer.h"
#include "RenderLayer/OpaqueShadowRenderLayer.h"
#include "RenderLayer/SelectRenderLayer.h"

FRenderLayerManage* FRenderLayerManage::RenderLayerManage = nullptr;
std::vector<std::shared_ptr<FRenderingLayer>> FRenderLayerManage::RenderingLayers;

FRenderLayerManage::~FRenderLayerManage()
{
	// ����ɾ����̬����ж�������������ǲ��Եģ���̬��Ա�������Լ����ڴ�λ�ú��������ڣ�ɾ����Ӧ��д������������
	// ��������������Ķ������ٵ�ʱ�����õ�,����Ҫע�⣬�Ͳ�������ʾ����
	// https://blog.csdn.net/weixin_51965272/article/details/123944686
	if (RenderLayerManage != nullptr)
	{
		delete RenderLayerManage;
	}

	RenderLayerManage = nullptr;
	RenderingLayers.clear();
}

FRenderLayerManage::FRenderLayerManage()
{
	RenderingLayers.clear();

	//�ڳ�ʼ��renderlayer��ʱ������Ĭ������һЩ�㼶
	CreateRenderLayer<FBackgroundRenderLayer>();
	CreateRenderLayer<FAlphaTestRenderLayer>();
	CreateRenderLayer<FOpaqueRenderLayer>();
	CreateRenderLayer<FTransparentRenderLayer>();
	CreateRenderLayer<FOpaqueReflectorRenderLayer>();
	CreateRenderLayer<FOpaqueShadowRenderLayer>();
	CreateRenderLayer<FSelectRenderLayer>();
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
	Engine_Log("No find Render Layer !!!");
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

void FRenderLayerManage::Draw(int inLayer, float DeltaTime)
{
	for (auto& tem : RenderingLayers)
	{
		if (tem->GetRenderLayerType() == inLayer)
		{
			tem->Draw(DeltaTime);
			break;
		}
	}
}

void FRenderLayerManage::PostDraw(float DeltaTime)
{
	for (auto& tem : RenderingLayers)
	{
		tem->PostDraw(DeltaTime);
	}
}

void FRenderLayerManage::DrawObjectByLayer(float DeltaTime, int InLayer, const CMeshComponent* InKey)
{
	for (auto& tem : RenderingLayers)
	{
		if (tem->GetRenderLayerType() == InLayer)
		{
			tem->DrawObjectByLayer(DeltaTime, InKey);
			break;
		}
	}
}

void FRenderLayerManage::ResetPSO(int InLayer)
{
	for (auto& tem : RenderingLayers)
	{
		if (tem->GetRenderLayerType() == InLayer)
		{
			tem->ResetPSO();
			break;
		}
	}
}

void FRenderLayerManage::DrawAllObjectsByLayer(float DeltaTime, int InLayer)
{
	for (auto& tem : RenderingLayers)
	{
		if (tem->GetRenderLayerType() == InLayer)
		{
			tem->DrawAllObjectsByLayer(DeltaTime);
			break;
		}
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

void FRenderLayerManage::AddGeometryDescData(int InLayer, std::weak_ptr<FGeometryDescData> InGeometryDescData)
{
	for (auto& tem : RenderingLayers)
	{
		if (tem->GetRenderLayerType() == InLayer)
		{
			tem->AddGeometryDescData(InGeometryDescData);
			break;
		}
	}
}

void FRenderLayerManage::RemoveGeometryDescData(int InLayer, std::weak_ptr<FGeometryDescData> InGeometryDescData)
{
	for (auto& tem : RenderingLayers)
	{
		if (tem->GetRenderLayerType() == InLayer)
		{
			tem->RemoveGeometryDescData(InGeometryDescData);
			break;
		}
	}
}

void FRenderLayerManage::ClearGeometryDescData(int InLayer)
{
	for (auto& tem : RenderingLayers)
	{
		if (tem->GetRenderLayerType() == InLayer)
		{
			tem->ClearGeometryDescData();
			break;
		}
	}
}

