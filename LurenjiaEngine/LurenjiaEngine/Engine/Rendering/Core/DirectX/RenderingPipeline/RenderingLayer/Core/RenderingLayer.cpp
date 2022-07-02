#include "RenderingLayer.h"
#include "../RenderLayerManage.h"

FRenderingLayer::FRenderingLayer()
	: RenderingLayerPriority(0)
{
}

void FRenderingLayer::RegisterRenderLayer()
{
	FRenderLayerManage::RenderingLayers.emplace_back(shared_from_this());
}
