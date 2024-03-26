#pragma once

#ifndef TransparentRenderLayer_H
#define TransparentRenderLayer_H

#include "../Core/RenderingLayer.h"

class FTransparentRenderLayer : public FRenderingLayer
{
	typedef FRenderingLayer super;
public:
	FTransparentRenderLayer();

	void BuildPSO() override;
	void BuildShader() override;
	void Draw(float DeltaTime) override;
	int GetRenderLayerType() const override;
};


#endif
