#pragma once

#ifndef RenderingLayer_H
#define RenderingLayer_H

#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/GeometryDescData.h"
#include "../../PiepelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

class FRenderingLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderingLayer>
{
public:
	FRenderingLayer();
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState);
	virtual int GetRenderLayerType() const;
	void RegisterRenderLayer();
	UINT GetPriority();
	std::vector<FGeometryDescData>* GetGeometryDescData();
	virtual void UpdateObjectConstantBuffer();
	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);
public:
	virtual void BuildShader() = 0;	//纯虚函数，必须实现方法
protected:
	UINT RenderingLayerPriority;
	FShader VertexShader;
	FShader PixelShader;
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		//着色器输入布局
	std::vector<FGeometryDescData> GeometryDescDatas;			//描述Getometry的数据

	FGeometryMap* GeometryMap;
	FDirectXPiepelineState* DirectXPiepelineState;
};


#endif // !RenderingLayer_H

