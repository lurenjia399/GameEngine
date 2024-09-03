#pragma once
#include "../CoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../Construction/ConstructionComponents.h"

class CCoreMinimalObject;

struct LURENJIACOREOBJECT_API FCreateObjectParam
{
	FCreateObjectParam(CCoreMinimalObject* InOuter, CCoreMinimalObject* InClass, const char* InName)
		: Outer(InOuter)
		, Class(InClass)
		, Name(InName)
	{}
	CCoreMinimalObject* Outer;
	CCoreMinimalObject* Class;
	const char* Name;				//对象的名称
};


//----------模板实现-----
namespace LurenjiaEngine
{
	template<typename T>
	T* CreateObject(const FCreateObjectParam& InObjectParam)
	{
		CCoreMinimalObject* NewObject = new T();	//创建对象

		//检测是不是组件 是组件按照组件规则注册
		ConstructionComponent::ConstructionComponents(InObjectParam.Outer, NewObject);

		T* Obj = dynamic_cast<T*>(NewObject);
		Obj->SetOuter(InObjectParam.Outer);
		Obj->ResetGuid(InObjectParam.Name);				//重新设置对象的guid
		Obj->InitMinimalObject();

		return Obj;
	}
	template<typename T>
	T* CreateObject(CCoreMinimalObject* Outer, string objName)
	{
		CCoreMinimalObject* NewObject = new T();	//创建对象

		//检测是不是组件 是组件按照组件规则注册
		ConstructionComponent::ConstructionComponents(Outer, NewObject);

		T* Obj = dynamic_cast<T*>(NewObject);
		Obj->SetOuter(Outer);
		Obj->ResetGuid(objName);				//重新设置对象的guid
		Obj->InitMinimalObject();

		return Obj;
	}
}
