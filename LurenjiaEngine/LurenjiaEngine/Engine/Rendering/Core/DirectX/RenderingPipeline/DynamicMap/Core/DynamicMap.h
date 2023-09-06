#pragma once 

#ifndef DYNAMICMAP_H
#define DYNAMICMAP_H

#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../RenderTarget/Core/RenderTarget.h"

struct FGeometryMap;
struct FDirectXPiepelineState;

class FDynamicMap : public IDirectXDeviceInterface
{
public:
	FDynamicMap(int inWidth, int inHeight, FGeometryMap* inGeometryMap, FDirectXPiepelineState* inDirectXPiepelineState);

public:
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* PSO);
	virtual void UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo) {};
	virtual void PreDraw(float DeltaTime) {};
	virtual void Draw(float DeltaTime) {};

	template<class T>
	void CreateRenderTarget();

protected:
	std::unique_ptr<FRenderTarget> RenderTarget;

	FGeometryMap* GeometryMap;
	FDirectXPiepelineState* PSO;

	UINT Width;
	UINT Height;
};

#endif

template<class T>
void FDynamicMap::CreateRenderTarget()
{
	if (!RenderTarget)
	{
		RenderTarget = std::make_unique<T>();
	}
	
}
