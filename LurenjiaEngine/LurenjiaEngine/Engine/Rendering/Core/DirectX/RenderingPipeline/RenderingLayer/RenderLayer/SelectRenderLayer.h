#pragma once

#ifndef SDELECTRENDERLAYER_H
#define SDELECTRENDERLAYER_H

#include "../Core/RenderingLayer.h"

class FSelectRenderLayer : public FRenderingLayer
{
	typedef FRenderingLayer Super;
public:
	FSelectRenderLayer();

	virtual void BuildShader() override;
	virtual void BuildPSO() override;
	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;
	virtual int GetRenderLayerType() const override;
};


#endif
