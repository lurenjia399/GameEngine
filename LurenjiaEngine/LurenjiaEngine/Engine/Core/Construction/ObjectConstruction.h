#pragma once

class CCoreMinimalObject;

namespace LurenjiaEngine
{
	//----------模板实现-----
	template<typename T>
	shared_ptr<T> CreateObject(std::weak_ptr<CCoreMinimalObject> InOuter, string objName)
	{
		//CCoreMinimalObject* NewObject = new T();	//创建对象
		std::shared_ptr<CCoreMinimalObject> NewObject = std::make_shared<T>();
		NewObject->BeginInit();
		NewObject->SetOuter(InOuter);
		NewObject->ResetGuid(objName);				//重新设置对象的guid

		return static_pointer_cast<T>(NewObject);
	}

	template<typename T, typename ...ParamTypes>
	T* ConstructionObject(CCoreMinimalObject* InOuter, ParamTypes&&... Params)
	{
		CCoreMinimalObject* NewObject = new T(Params...);
		//还需要BeginInit
		//还需要设置Outer
		//还需要设置guid

		return dynamic_cast<T*>(NewObject);
	}
}
