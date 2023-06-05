#pragma once
#include "../../Core/Engine.h"

class FViewport
{
public:
	FViewport();
	void ViewportInit();
	XMFLOAT4X4 ViewMatrix;//ÉãÏñ»ú¾ØÕó
	XMFLOAT4X4 ProjectMatrix;//Í¶Ó°¾ØÕó
};