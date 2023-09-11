#pragma once 

#ifndef DYNAMICSHADOWMAP_H
#define DYNAMICSHADOWMAP_H

#include "../Core/DynamicMap.h"

class FDynamicShadowMap : public FDynamicMap
{
	typedef FDynamicMap Super;
public:
	FDynamicShadowMap();

	virtual void UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo) override;
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState) override;
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
};

#endif