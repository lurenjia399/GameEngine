#pragma once

#include "GuidInterface.h"

class LURENJIACOREOBJECT_API CCoreMinimalObject : public IGuidInterface//, public enable_shared_from_this<CCoreMinimalObject>
{
public:
	virtual void BeginInit() {}
	virtual void Tick(float DeltaTime) {}
public:

	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	bool IsTick()const { return isCanTick; }
	void SetOuter(CCoreMinimalObject* InOuter) { Outer = InOuter; }
	CCoreMinimalObject* GetOuter() {return Outer;}
protected:
	bool isCanTick;
	CCoreMinimalObject* Outer; // 当前这个对象的上层对象
};

extern vector<CCoreMinimalObject*> ObjectPool;