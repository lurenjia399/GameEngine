#pragma once
#include "../../Core/Engine.h"

class FViewport
{
public:
	FViewport();
	void ViewportInit();

	void ResetViewport(UINT InWidth, UINT InHeight);
	void ResetScissorRect(UINT InWidth, UINT InHeight);

	D3D12_VIEWPORT ViewPortInfo;
	D3D12_RECT ViewPortRect;

	XMFLOAT4X4 ViewMatrix;//…„œÒª˙æÿ’Û
	XMFLOAT4X4 ProjectMatrix;//Õ∂”∞æÿ’Û
};