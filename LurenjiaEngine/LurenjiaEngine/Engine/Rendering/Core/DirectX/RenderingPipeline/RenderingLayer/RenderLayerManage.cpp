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

	//�ڳ�ʼ��renderlayer��ʱ������Ĭ�����һЩ�㼶
	CreateRenderLayer<FAlphaTestRenderLayer>();
	CreateRenderLayer<FOpaqueRenderLayer>();
	CreateRenderLayer<FTransparentRenderLayer>();
}


