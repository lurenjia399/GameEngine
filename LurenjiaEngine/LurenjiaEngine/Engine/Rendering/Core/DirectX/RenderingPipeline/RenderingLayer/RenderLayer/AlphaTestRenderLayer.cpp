#include "AlphaTestRenderLayer.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
	RenderingLayerPriority = 2450;
}

void FAlphaTestRenderLayer::BuildShader()
{
}

int FAlphaTestRenderLayer::GetRenderLayerType() const
{
	return 2;
}
