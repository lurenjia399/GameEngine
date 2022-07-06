#pragma once

#ifndef OpaqueRenderLayer_H
#define OpaqueRenderLayer_H

#include "../Core/RenderingLayer.h"

class FOpaqueRenderLayer : public FRenderingLayer
{
public:
	FOpaqueRenderLayer();

	virtual void BuildShader() override;
	void BuildPSO() override;

	int GetRenderLayerType() const override;
	
};


#endif
