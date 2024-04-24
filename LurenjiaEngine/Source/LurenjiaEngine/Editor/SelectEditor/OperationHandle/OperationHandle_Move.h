#pragma once
#include "Core/OperationHandleBase.h"

class AOperationHandle_Move : public AOperationHandleBase
{
	typedef AOperationHandleBase Super;
public:
	AOperationHandle_Move();
	void CreateComponent();
};