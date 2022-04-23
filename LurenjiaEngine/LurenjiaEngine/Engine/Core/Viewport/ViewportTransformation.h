#pragma once
#include "../../Core/Engine.h"

struct FViewportTransformation
{
	FViewportTransformation();
	XMFLOAT4X4 ViewProjectionMatrix;
};