#pragma once
struct FEngineRenderConfig
{
	int ScreenWidth;
	int ScreenHeight;
	int RefreshRate;
	int SwapChainCount;

	static FEngineRenderConfig* GetRenderConfig();
	static void Destroy();
private:
	FEngineRenderConfig();
	static FEngineRenderConfig* RenderConfig;
};