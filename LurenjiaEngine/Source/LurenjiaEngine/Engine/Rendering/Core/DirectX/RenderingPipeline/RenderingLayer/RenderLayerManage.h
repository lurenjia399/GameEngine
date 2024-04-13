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
	void Draw(int inLayer, float DeltaTime);// // drawĳ���㼶�е�����Object
	void PostDraw(float DeltaTime);
	// �ҵ��㼶��ĳ��object
	void DrawObjectByLayer(float DeltaTime, int InLayer, const CMeshComponent* InKey);
	// drawĳ���㼶�е�����Object�������draw������ô��������Draw����Ҫʹ��layer��Ӧ��pso�����������
	void DrawAllObjectsByLayer(float DeltaTime, int InLayer); 
	// resetĳ���㼶��pso
	void ResetPSO(int InLayer);
	
	void UpdateObjectConstantBuffer();
	void BuildPSO();

	void AddGeometryDescData(int InLayer, std::weak_ptr<FGeometryDescData> InGeometryDescData);
	void RemoveGeometryDescData(int InLayer, std::weak_ptr<FGeometryDescData> InGeometryDescData);
	void ClearGeometryDescData(int InLayer);
private:
	FRenderLayerManage();

	~FRenderLayerManage();// ע������������������⣬����Ͳ����ˣ���ʾ����
	
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