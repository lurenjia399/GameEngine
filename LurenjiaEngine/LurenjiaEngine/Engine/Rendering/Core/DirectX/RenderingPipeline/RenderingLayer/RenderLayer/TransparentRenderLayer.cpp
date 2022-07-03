#include "TransparentRenderLayer.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
	RenderingLayerPriority = 3000;
}

void FTransparentRenderLayer::BuildShader()
{
}

int FTransparentRenderLayer::GetRenderLayerType() const
{
	return 1;
}
