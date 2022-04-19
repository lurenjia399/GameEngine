#pragma once
#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"

class CCamera;

class CWord : public CCoreMinimalObject
{
public:
	CWord();

	CVARIABLE()
	CCamera* camera;
};