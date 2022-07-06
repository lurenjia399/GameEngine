#pragma once

class CCoreMinimalObject;

namespace LurenjiaEngine
{
	//----------ģ��ʵ��-----
	template<typename T>
	T* CreateObject(string objName)
	{
		CCoreMinimalObject* NewObject = new T();	//��������
		NewObject->ResetGuid(objName);				//�������ö����guid

		return dynamic_cast<T*>(NewObject);
	}

	template<typename T, typename ...ParamTypes>
	T* ConstructionObject(ParamTypes&&... Params)
	{
		CCoreMinimalObject* NewObject = new T(Params...);
		//����Ҫ����guid

		return dynamic_cast<T*>(NewObject);
	}
}
