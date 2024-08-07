#include "FuntionManager.h"
#include "../CoreObject/Function.h"

namespace NativeFunctionMess
{
	void Error(CCoreMinimalObject* Context, FFrame* StackFrame, void const* RefData)
	{
		assert(0);
	}
	void Empty(CCoreMinimalObject* Context, FFrame* StackFrame, void const* RefData)
	{

	}
}

FNativeFuntionPtr ErrorFunction = NativeFunctionMess::Error;
FNativeFuntionPtr EmptyFunction = NativeFunctionMess::Empty;

vector<FFunctionID> FFunctionManage::FunctionList;

FNativeFuntionPtr& FFunctionManage::GetNativeFuncPtr(UFunction* InFunction)
{
	for (FFunctionID& Func : FunctionList)
	{
		if (InFunction->GetName() == Func.FuncName &&
			InFunction->GetOuter()->GetName() == Func.ClassName)
		{
			return Func.NativeFuncPtr;
		}
	}
	return EmptyFunction;
}

int FFunctionManage::SetNativeFuncPtr(const FFunctionID& InFFunctionID)
{
	FunctionList.push_back(InFFunctionID);
}
