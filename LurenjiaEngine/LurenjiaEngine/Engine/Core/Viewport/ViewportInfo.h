#pragma once
#include "../../Core/Engine.h"

struct FViewportInfo
{
	XMFLOAT4 cameraPosition;
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectMatrix;
};