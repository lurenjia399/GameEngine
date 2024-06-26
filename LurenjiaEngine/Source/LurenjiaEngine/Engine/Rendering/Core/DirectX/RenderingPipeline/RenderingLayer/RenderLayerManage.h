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
	static void Destory()
	{
		if (RenderLayerManage != nullptr)
		{
			delete RenderLayerManage;
		}

		RenderLayerManage = nullptr;
		RenderingLayers.clear();
	}
	static std::shared_ptr<FRenderingLayer> FindRenderingLayerByInt(int InRenderLayer);
	void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState);
	void BuildShader();
	void sort();

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void Draw(int inLayer, float DeltaTime);// // draw某个层级中的所有Object
	void PostDraw(float DeltaTime);
	// 找到层级中某个object
	void DrawObjectByLayer(float DeltaTime, int InLayer, const CMeshComponent* InKey);
	// draw某个层级中的所有Object，这个和draw有区别么，区别是Draw必须要使用layer对应的pso，而这个不用
	void DrawAllObjectsByLayer(float DeltaTime, int InLayer); 
	// reset某个层级的pso
	void ResetPSO(int InLayer);
	
	void UpdateObjectConstantBuffer();
	void BuildPSO();

	void AddGeometryDescData(int InLayer, std::weak_ptr<FGeometryDescData> InGeometryDescData);
	void RemoveGeometryDescData(int InLayer, std::weak_ptr<FGeometryDescData> InGeometryDescData);
	void ClearGeometryDescData(int InLayer);
private:
	FRenderLayerManage();

	~FRenderLayerManage();// 注意这个析构函数有问题，这里就不改了，以示提醒
	
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
