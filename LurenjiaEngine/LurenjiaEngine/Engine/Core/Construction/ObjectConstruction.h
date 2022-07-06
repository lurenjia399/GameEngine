#pragma once

class CCoreMinimalObject;

namespace LurenjiaEngine
{
	//----------模板实现-----
	template<typename T>
	T* CreateObject(string objName)
	{
		CCoreMinimalObject* NewObject = new T();	//创建对象
		NewObject->ResetGuid(objName);				//重新设置对象的guid

		return dynamic_cast<T*>(NewObject);
	}

	template<typename T, typename ...ParamTypes>
	T* ConstructionObject(ParamTypes&&... Params)
	{
		CCoreMinimalObject* NewObject = new T(Params...);
		//还需要设置guid

		return dynamic_cast<T*>(NewObject);
	}
}
