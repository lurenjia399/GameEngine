#pragma once


class CCoreMinimalObject;

namespace LurenjiaEngine
{
	//#include "../CoreObject/CoreMinimalObject.h"

	struct FStaticConstructObjectParameters
	{
		FStaticConstructObjectParameters(CCoreMinimalObject* InOuter, const shared_ptr<CCoreMinimalObject> InClass, const string InName) 
			: Outer(InOuter)
			, Class(InClass)
			, Name(InName)
		{}
		CCoreMinimalObject* Outer;
		std::shared_ptr<CCoreMinimalObject> Class;
		string Name;				//对象的名称
	};

	//std::shared_ptr<CCoreMinimalObject> StaticConstructObject_Internal(const FStaticConstructObjectParameters& Params)
	//{
	//	shared_ptr<CCoreMinimalObject> NewClass = Params.Class;
	//	NewClass->SetOuter(Params.Outer);
	//	NewClass->ResetGuid(Params.Name);

	//	return NewClass;
	//}


	//----------模板实现-----
	template<typename T>
	shared_ptr<T> CreateObject(CCoreMinimalObject* InOuter, string objName)
	{
		//CCoreMinimalObject* NewObject = new T();	//创建对象
		const shared_ptr<CCoreMinimalObject> NewObject = std::make_shared<T>();
		NewObject->SetOuter(InOuter);
		NewObject->ResetGuid(objName);				//重新设置对象的guid
		FStaticConstructObjectParameters Params = FStaticConstructObjectParameters(InOuter, NewObject, objName);

		return static_pointer_cast<T>(NewObject);
	}

	template<typename T, typename ...ParamTypes>
	T* ConstructionObject(CCoreMinimalObject* InOuter, ParamTypes&&... Params)
	{
		CCoreMinimalObject* NewObject = new T(Params...);
		//还需要设置Outer
		//还需要设置guid

		return dynamic_cast<T*>(NewObject);
	}
}
