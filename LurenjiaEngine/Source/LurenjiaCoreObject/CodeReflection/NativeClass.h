#pragma once

#include <string>
#include <map>
#include "../CoreObjectMacro.h"
//#include "../CoreObjectMinimal.h"
//#include "../CoreObject/CoreMinimalObject.h"

// 不抛出4251警告
#pragma warning(push)
#pragma warning(disable:4251)

class CCoreMinimalObject;
class UFunction;
class UProperty;
struct LURENJIACOREOBJECT_API FNativeClass
{
public:
	FNativeClass();

	void AddProperty(
		const std::string& PropertyName,	//属性名称
		const std::string& PropertyType,	//属性类型
		int PropertyCount,			//属性数量
		int PerPropertySize,		//属性大小
		void* InData);				//属性存储的首地址
public:
	CCoreMinimalObject* Outer;
	UProperty* Property;
	/** Map of all functions by name contained in this class */
	std::map<std::string, UFunction*> FunctionMap;

};

#pragma warning(pop)
