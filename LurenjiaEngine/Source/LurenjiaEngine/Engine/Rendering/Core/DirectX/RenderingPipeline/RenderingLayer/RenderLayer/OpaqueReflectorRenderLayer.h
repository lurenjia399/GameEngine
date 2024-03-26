#pragma once

#ifndef OpaqueReflectorRenderLayer_H
#define OpaqueReflectorRenderLayer_H

#include "../Core/RenderingLayer.h"

class FOpaqueReflectorRenderLayer : public FRenderingLayer
{
	typedef FRenderingLayer super;
public:
	FOpaqueReflectorRenderLayer();

	void BuildShader() override;
	void BuildPSO() override;
	void Draw(float DeltaTime) override;
	int GetRenderLayerType() const override;
};

#endif
