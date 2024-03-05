#pragma once

#include "GuidInterface.h"
#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"		//����ͷ�ļ�
#include "../Construction/ObjectConstruction.h"
#include "../Construction/SingleInstanceConstruction.h"

class CCoreMinimalObject : public IGuidInterface, public enable_shared_from_this<CCoreMinimalObject>
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
	CCoreMinimalObject* Outer; // ��ǰ���������ϲ����
};

extern vector<CCoreMinimalObject*> ObjectPool;