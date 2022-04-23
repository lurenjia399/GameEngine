#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"

class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	CCamera* GetCamera()const { return camera; }
private:
	CVARIABLE()
	CCamera* camera;
};