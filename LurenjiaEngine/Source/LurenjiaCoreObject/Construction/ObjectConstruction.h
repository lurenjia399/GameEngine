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
template<typename T>
T* CreateObject(const FCreateObjectParam& InObjectParam, string objName)
{
	CCoreMinimalObject* NewObject = new T();	//创建对象

	//检测是不是组件 是组件按照组件规则注册
	ConstructionComponent::ConstructionComponents(InObjectParam.Outer, NewObject);

	T* Obj = dynamic_cast<T*>(NewObject);
	Obj->SetOuter(InObjectParam.Outer);
	Obj->ResetGuid(objName);				//重新设置对象的guid

	return Obj;
}

//template<typename T, typename ...ParamTypes>
//T* ConstructionObject(CCoreMinimalObject* InOuter, ParamTypes&&... Params)
//{
//	CCoreMinimalObject* NewObject = new T(Params...);
//	//还需要设置Outer
//	//还需要设置guid
//
//	return dynamic_cast<T*>(NewObject);
//}
