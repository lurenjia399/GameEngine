#pragma once
#include "../../LurenjiaEngine.h"

enum EPressState
{
	PRESS,	//����
	RELEASE	//�ɿ�
};

struct FInputKey
{
	FInputKey();

	string KeyName;
	EPressState PressState;
};