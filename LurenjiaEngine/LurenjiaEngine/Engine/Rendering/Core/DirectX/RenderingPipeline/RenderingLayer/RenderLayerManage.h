#pragma once

#ifndef RenderLayerManage_H
#define RenderLayerManage_H

#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "Core/RenderingLayer.h"

class FRenderLayerManage
{
	friend class FRenderingLayer;
	friend struct FGeometry;
	friend struct FGeometryMap;
public:
	static FRenderLayerManage* GetRenderLayerManage()
	{
		if (RenderLayerManage == nullptr)
		{
			RenderLayerManage = new FRenderLayerManage();
		}
		return RenderLayerManage;
	}
	static std::shared_ptr<FRenderingLayer> FindRenderingLayerByInt(int InRenderLayer);
	void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState);
	void BuildShader();
	void sort();
	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void Draw(int inLayer, float DeltaTime);
	void PostDraw(float DeltaTime);
	void UpdateObjectConstantBuffer();
	void BuildPSO();
private:
	FRenderLayerManage();
	~FRenderLayerManage();
	
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
