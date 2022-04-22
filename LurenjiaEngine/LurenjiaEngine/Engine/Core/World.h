#pragma once
#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"

class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();

	CVARIABLE()
	CCamera* camera;
};