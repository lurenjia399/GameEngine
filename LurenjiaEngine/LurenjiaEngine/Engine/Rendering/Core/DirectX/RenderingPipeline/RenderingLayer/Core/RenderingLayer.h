#pragma once

#ifndef RenderingLayer_H
#define RenderingLayer_H

#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/GeometryDescData.h"
#include "../../PiepelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"
#include "../../../../../../Shader/Core/ShaderType.h"

class FRenderingLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderingLayer>
{
public:
	FRenderingLayer();
	void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState);
	void RegisterRenderLayer();
	UINT GetPriority();
	std::vector<FGeometryDescData>* GetGeometryDescData();
	void RestorePSO();
	virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& OutShaderMacro);
	virtual void UpdateObjectConstantBuffer();
	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);
	virtual void BuildPSO();
	virtual void DrawObject(float DeltaTime, const FGeometryDescData& InRenderingData);
	virtual void FindObjectDraw(float DeltaTime, const CMeshComponent* InKey);
public:
	virtual void BuildShader() = 0;	//纯虚函数，必须实现方法
	virtual int GetRenderLayerType() const = 0; // 纯虚函数，必须实现方法
protected:
	UINT RenderingLayerPriority;
	FShader VertexShader;
	FShader PixelShader;
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		//着色器输入布局
	std::vector<FGeometryDescData> GeometryDescDatas;			//描述Getometry的数据

	FGeometryMap* GeometryMap;
	FDirectXPiepelineState* DirectXPiepelineState;
};


#endif

