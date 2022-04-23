#pragma once
#include "../../Core/Engine.h"

class FViewport
{
public:
	FViewport();
	void ViewportInit();
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectMatrix;
};