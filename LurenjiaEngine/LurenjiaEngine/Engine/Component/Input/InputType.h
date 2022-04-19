#pragma once
#include "../../LurenjiaEngine.h"

enum EPressState
{
	PRESS,	//╟╢об
	RELEASE	//ки©╙
};

struct FInputKey
{
	FInputKey();

	string KeyName;
	EPressState PressState;
};