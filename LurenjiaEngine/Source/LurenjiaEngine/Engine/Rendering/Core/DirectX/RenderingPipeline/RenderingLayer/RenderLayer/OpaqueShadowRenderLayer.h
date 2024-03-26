#pragma once

#ifndef OpaqueShadowRenderLayer_H
#define OpaqueShadowRenderLayer_H

#include "../Core/RenderingLayer.h"

class FOpaqueShadowRenderLayer : public FRenderingLayer
{
	typedef FRenderingLayer super;
public:
	FOpaqueShadowRenderLayer();

	void BuildShader() override;
	void BuildPSO() override;
	void Draw(float DeltaTime) override;
	int GetRenderLayerType() const override;
	void ResetPSO() override;
};


#endif
