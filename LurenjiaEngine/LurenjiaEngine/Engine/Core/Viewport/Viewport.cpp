#include "Viewport.h"
#include "../../Math/EngineMath.h"
#include "../../Config/EngineRenderConfig.h"

FViewport::FViewport()
	:ViewMatrix(EngineMath::IdentityMatrix4x4())
	,ProjectMatrix(EngineMath::IdentityMatrix4x4())
{
}

void FViewport::ViewportInit()
{
	float AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	XMMATRIX Project = XMMatrixPerspectiveFovLH(0.25f * XM_PI, AspectRatio, 1.0f, 1000.f);
	XMStoreFloat4x4(&ProjectMatrix, Project);
}
