#include "EngineRenderConfig.h"



FEngineRenderConfig::FEngineRenderConfig():
	ScreenWidth(1280),
	ScreenHeight(720),
	RefreshRate(60),
	SwapChainCount(2)
{
}

FEngineRenderConfig* FEngineRenderConfig::GetRenderConfig()
{
	if (!RenderConfig)
	{
		RenderConfig = new FEngineRenderConfig();
	}
	return RenderConfig;
}

void FEngineRenderConfig::Destroy()
{
	if (RenderConfig)
	{
		delete RenderConfig;
		RenderConfig = nullptr;
	}
}
