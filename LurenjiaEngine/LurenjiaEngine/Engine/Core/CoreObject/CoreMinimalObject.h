#pragma once

#include "GuidInterface.h"
#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"		//����ͷ�ļ�

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

//----------ģ��ʵ��-----
template<typename T>
T* CCoreMinimalObject::CreateObject(string objName)
{
	CCoreMinimalObject* NewObject = new T();	//��������
	NewObject->ResetGuid(objName);				//�������ö����guid

	return dynamic_cast<T*>(NewObject);
}