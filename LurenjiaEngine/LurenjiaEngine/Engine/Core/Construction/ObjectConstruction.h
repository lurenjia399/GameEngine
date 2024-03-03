#pragma once

class CCoreMinimalObject;

namespace LurenjiaEngine
{
	//----------ģ��ʵ��-----
	template<typename T>
	shared_ptr<T> CreateObject(std::weak_ptr<CCoreMinimalObject> InOuter, string objName)
	{
		//CCoreMinimalObject* NewObject = new T();	//��������
		std::shared_ptr<CCoreMinimalObject> NewObject = std::make_shared<T>();
		NewObject->BeginInit();
		NewObject->SetOuter(InOuter);
		NewObject->ResetGuid(objName);				//�������ö����guid

		return static_pointer_cast<T>(NewObject);
	}

	template<typename T, typename ...ParamTypes>
	T* ConstructionObject(CCoreMinimalObject* InOuter, ParamTypes&&... Params)
	{
		CCoreMinimalObject* NewObject = new T(Params...);
		//����ҪBeginInit
		//����Ҫ����Outer
		//����Ҫ����guid

		return dynamic_cast<T*>(NewObject);
	}
}
