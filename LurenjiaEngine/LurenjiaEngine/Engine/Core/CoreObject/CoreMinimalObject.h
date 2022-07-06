#pragma once

#include "GuidInterface.h"
#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"		//反射头文件
#include "../Construction/ObjectConstruction.h"

class CCoreMinimalObject : public IGuidInterface
{
public:
	virtual void BeginInit() {}
	virtual void Tick(float DeltaTime) {}
public:

	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	bool IsTick()const { return isCanTick; }
protected:
	bool isCanTick;
};

extern vector<CCoreMinimalObject*> ObjectPool;