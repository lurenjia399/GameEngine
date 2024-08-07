#pragma once

#include "../CoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "Frame.h"

class UFunction;

struct LURENJIACOREOBJECT_API FFunctionID
{
	std::string ClassName;//类名
	std::string FuncName;//函数名

	FNativeFuntionPtr NativeFuncPtr;//函数指针

	FFunctionID(const std::string& InNewClassName, const std::string& InNewFuncName, FNativeFuntionPtr InNewFunctionPtr)
		: ClassName(InNewClassName)
		, FuncName(InNewFuncName)
		, NativeFuncPtr(InNewFunctionPtr)
	{
	}
};

struct LURENJIACOREOBJECT_API FFunctionManage
{
	static FNativeFuntionPtr& GetNativeFuncPtr(UFunction* InFunction);
	static int SetNativeFuncPtr(const FFunctionID& InFFunctionID);
private:
	static vector<FFunctionID> FunctionList;
};