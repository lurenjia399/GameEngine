#pragma once

#ifndef AlphaTestRenderLayer_H
#define AlphaTestRenderLayer_H

#include "../Core/RenderingLayer.h"

class FAlphaTestRenderLayer : public FRenderingLayer
{
	typedef FRenderingLayer super;
public:
	FAlphaTestRenderLayer();

	void BuildShader() override;

	int GetRenderLayerType() const override;
};


#endif
