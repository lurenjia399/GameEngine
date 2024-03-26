#include "Viewport.h"
#include "../../Math/EngineMath.h"
#include "../../Config/EngineRenderConfig.h"

FViewport::FViewport()
	: ViewMatrix(EngineMath::IdentityMatrix4x4())
	, ProjectMatrix(EngineMath::IdentityMatrix4x4())
	, ViewPortInfo({})
	, ViewPortRect({})
{
	ResetViewport(FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
	ResetScissorRect(FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
}

void FViewport::ViewportInit()
{
	float AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	XMMATRIX Project = XMMatrixPerspectiveFovLH(0.25f * XM_PI, AspectRatio, 1.0f, 1000.f);
	XMStoreFloat4x4(&ProjectMatrix, Project);
}

void FViewport::ResetViewport(UINT InWidth, UINT InHeight)
{
	ViewPortInfo = {
		0.f,			// TopLeftX
		0.f,			// TopLeftY
		(float)InWidth,	// Width
		(float)InHeight,// Height
		0.f,			// MinDepth
		1.f				// MaxDepth
	};
}

void FViewport::ResetScissorRect(UINT InWidth, UINT InHeight)
{
	ViewPortRect = {
		0,				// left
		0,				// top
		(LONG)InWidth,	// right
		(LONG)InHeight	// bottom
	};
}