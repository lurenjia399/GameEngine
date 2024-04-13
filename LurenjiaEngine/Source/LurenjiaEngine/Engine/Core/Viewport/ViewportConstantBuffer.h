#pragma once
#include "../../Core/Engine.h"

struct FViewportConstantBuffer
{
	FViewportConstantBuffer();
	XMFLOAT4 cameraPosition;
	XMFLOAT4X4 ViewProjectionMatrix;
};