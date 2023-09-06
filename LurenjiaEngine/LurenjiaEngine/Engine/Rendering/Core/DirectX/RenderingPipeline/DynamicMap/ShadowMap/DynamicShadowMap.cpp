#include "DynamicShadowMap.h"
#include "../../RenderTarget/ShadowMapRenderTarget.h"

FDynamicShadowMap::FDynamicShadowMap()
	: Super(512, 512, nullptr, nullptr)
{
	//CreateRenderTarget<FShadowMapRenderTarget>();
}
