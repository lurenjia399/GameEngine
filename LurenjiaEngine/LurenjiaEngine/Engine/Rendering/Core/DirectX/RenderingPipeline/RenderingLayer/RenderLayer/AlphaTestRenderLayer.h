#pragma once

#ifndef AlphaTestRenderLayer_H
#define AlphaTestRenderLayer_H

#include "../Core/RenderingLayer.h"

class FAlphaTestRenderLayer : public FRenderingLayer
{
public:
	FAlphaTestRenderLayer();

	virtual void BuildShader() override;

	int GetRenderLayerType() const override;
};


#endif // !AlphaTestRenderLayer
