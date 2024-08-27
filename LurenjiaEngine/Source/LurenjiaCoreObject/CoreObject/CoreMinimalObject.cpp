#include "CoreMinimalObject.h"
#include "Function.h"
#include "Property.h"
#include "../CodeReflection/Frame.h"

vector<CCoreMinimalObject*> ObjectPool;

void CCoreMinimalObject::InitMinimalObject()
{
	NativeClass.Outer = this;

	InitReflectionContent();
}

CCoreMinimalObject::CCoreMinimalObject()
{
	isCanTick = true;
	ObjectPool.emplace_back(this);
	Outer = {};
}

CCoreMinimalObject::~CCoreMinimalObject()
{
	for (vector<CCoreMinimalObject*>::const_iterator it = ObjectPool.begin(); it != ObjectPool.end(); it++)
	{
		if (*it == this)
		{
			ObjectPool.erase(it);
			break;
		}
	}
}

UFunction* CCoreMinimalObject::FindFunctionByName(std::string InName) const
{
	UFunction* NewFunction = nullptr;
	auto It = NativeClass.FunctionMap.find(InName);
	if (It != NativeClass.FunctionMap.end())
	{
		NewFunction = It->second;
	}
	return NewFunction;
}

void CCoreMinimalObject::CallFunction(FFrame& Stack, void const* Data, UFunction* Function)
{
	// 拿到首地址，从Function里面拿到的Property就是参数
	UProperty* PropertyPtr = Function->FirstPropertyToInit;
	// 收集函数参数，把参数都放到Frame的OutParam里面
	while (PropertyPtr != nullptr)
	{
		Stack.AddOutParam(PropertyPtr);
		PropertyPtr = dynamic_cast<UProperty*>(PropertyPtr->Next);
	}

	// 执行字节码
	Stack.Step(nullptr, Data);
}

void CCoreMinimalObject::ProcessEvent(UFunction* Function, void* Parms)
{
	assert(Function);

	// Function里面没有任何东西，直接返回，不执行了
	if (Function->Script->size() == 0)
	{
		return;
	}

	FFrame Stack(Function);

	// 直接调用方法，Frame,函数的参数,函数本身
	CCoreMinimalObject::CallFunction(Stack, Parms, Function);
}
