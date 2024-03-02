#pragma once

#include "GuidInterface.h"
#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"		//����ͷ�ļ�
#include "../Construction/ObjectConstruction.h"
#include "../Construction/SingleInstanceConstruction.h"

class CCoreMinimalObject : public IGuidInterface, public std::enable_shared_from_this<CCoreMinimalObject>
{
public:
	virtual void BeginInit() {}
	virtual void Tick(float DeltaTime) {}
public:

	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	bool IsTick()const { return isCanTick; }
	void SetOuter(std::weak_ptr<CCoreMinimalObject> InOuter) { Outer = InOuter; }
	std::weak_ptr<CCoreMinimalObject> GetOuter() {return Outer;}
protected:
	bool isCanTick;
	std::weak_ptr<CCoreMinimalObject> Outer; // ��ǰ���������ϲ����
};

extern vector<CCoreMinimalObject*> ObjectPool;