#pragma once
#include "../../Core/Engine.h"

struct FViewportTransformation
{
	FViewportTransformation();
	XMFLOAT4 cameraPosition;
	XMFLOAT4X4 ViewProjectionMatrix;
};