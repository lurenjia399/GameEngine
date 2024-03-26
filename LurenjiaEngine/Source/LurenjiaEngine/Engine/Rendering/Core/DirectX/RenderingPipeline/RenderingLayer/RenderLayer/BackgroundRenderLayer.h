#pragma once

#ifndef BackgroundRenderLayer_H
#define BackgroundRenderLayer_H

#include "../Core/RenderingLayer.h"

class FBackgroundRenderLayer : public FRenderingLayer
{
	typedef FRenderingLayer super;
public:
	FBackgroundRenderLayer();
public:
	void BuildShader() override;
	int GetRenderLayerType() const override;
public:
	void UpdateObjectConstantBuffer() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildPSO() override;
};

#endif