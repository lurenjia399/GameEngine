#include "DynamicShadowMap.h"
#include "../../RenderTarget/ShadowMapRenderTarget.h"

FDynamicShadowMap::FDynamicShadowMap()
	: Super(512, 512, nullptr, nullptr)
{
	//CreateRenderTarget<FShadowMapRenderTarget>();
}

void FDynamicShadowMap::UpdateViewportConstantBufferView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
}

void FDynamicShadowMap::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	Super::Init(InGeometryMap, InDirectXPiepelineState);
}

void FDynamicShadowMap::PreDraw(float DeltaTime)
{
}

void FDynamicShadowMap::Draw(float DeltaTime)
{
}
