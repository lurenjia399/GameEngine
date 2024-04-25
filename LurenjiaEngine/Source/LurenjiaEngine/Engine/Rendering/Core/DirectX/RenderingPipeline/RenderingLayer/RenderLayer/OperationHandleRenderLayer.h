#pragma once

#ifndef OperationHandleRenderLayer_H
#define OperationHandleRenderLayer_H

#include "../Core/RenderingLayer.h"

class FOperationHandleRenderLayer : public FRenderingLayer
{
	typedef FRenderingLayer super;
public:
	FOperationHandleRenderLayer();

	void BuildPSO() override;
	void BuildShader() override;
	void Draw(float DeltaTime) override;
	int GetRenderLayerType() const override;
};


#endif
