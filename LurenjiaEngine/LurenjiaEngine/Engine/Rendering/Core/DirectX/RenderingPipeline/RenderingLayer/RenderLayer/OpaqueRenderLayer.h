#pragma once

#ifndef OpaqueRenderLayer_H
#define OpaqueRenderLayer_H

#include "../Core/RenderingLayer.h"

class FOpaqueRenderLayer : public FRenderingLayer
{
	typedef FRenderingLayer super;
public:
	FOpaqueRenderLayer();

	void BuildShader() override;
	void BuildPSO() override;
	void Draw(float DeltaTime) override;;
	int GetRenderLayerType() const override;
	
};


#endif
