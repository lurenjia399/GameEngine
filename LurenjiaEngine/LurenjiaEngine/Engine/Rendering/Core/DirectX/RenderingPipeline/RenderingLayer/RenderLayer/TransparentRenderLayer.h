#pragma once

#ifndef TransparentRenderLayer_H
#define TransparentRenderLayer_H

#include "../Core/RenderingLayer.h"

class FTransparentRenderLayer : public FRenderingLayer
{
public:
	FTransparentRenderLayer();

	virtual void BuildShader() override;

	int GetRenderLayerType() const override;
};


#endif
