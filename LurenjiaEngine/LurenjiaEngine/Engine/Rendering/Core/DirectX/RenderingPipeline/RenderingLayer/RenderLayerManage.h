#pragma once

#ifndef RenderLayerManage_H
#define RenderLayerManage_H

#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "Core/RenderingLayer.h"

class FRenderLayerManage
{
	friend class FRenderingLayer;
public:
	static FRenderLayerManage* GetRenderLayerManage()
	{
		if (RenderLayerManage == nullptr)
		{
			RenderLayerManage = new FRenderLayerManage();
		}
		return RenderLayerManage;
	}
	~FRenderLayerManage();
private:
	FRenderLayerManage();
	static FRenderLayerManage* RenderLayerManage;
	static std::vector<std::shared_ptr<FRenderingLayer>> RenderingLayers;
};

#endif

template<typename T>
std::shared_ptr<T> CreateRenderLayer()
{
	std::shared_ptr<T> renderLayer = make_shared<T>();
	renderLayer->RegisterRenderLayer();


	return renderLayer;
}
