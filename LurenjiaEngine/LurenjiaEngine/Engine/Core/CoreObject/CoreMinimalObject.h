#pragma once

#include "GuidInterface.h"
#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"		//反射头文件

class CCoreMinimalObject : public IGuidInterface
{
public:
	virtual void BeginInit() {}
	virtual void Tick(float DeltaTime) {}
public:

	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	template<typename T>
	T* CreateObject(string str);

	bool IsTick()const { return isCanTick; }
protected:
	bool isCanTick;
};

extern vector<CCoreMinimalObject*> ObjectPool;

//----------模板实现-----
template<typename T>
T* CCoreMinimalObject::CreateObject(string objName)
{
	CCoreMinimalObject* NewObject = new T();	//创建对象
	NewObject->ResetGuid(objName);				//重新设置对象的guid

	return dynamic_cast<T*>(NewObject);
}