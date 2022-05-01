#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"

class ACamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	ACamera* GetCamera()const { return camera; }
private:
	CVARIABLE()
	ACamera* camera;
};