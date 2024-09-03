#pragma once

#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../Construction/ObjectConstruction.h"
#include "../CodeReflection/ScriptCommand.h"
#include "../CodeReflection/ScriptMacro.h"
#include "../CodeReflection/NativeClass.h"

// 不抛出4251警告
#pragma warning(push)
#pragma warning(disable:4251)
class UFunction;
class LURENJIACOREOBJECT_API CCoreMinimalObject : public IGuidInterface//, public enable_shared_from_this<CCoreMinimalObject>
{
public:
	virtual void BeginInit() {}
	virtual void Tick(float DeltaTime) {}
	virtual void InitMinimalObject();
public:

	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	bool IsTick()const { return isCanTick; }
	void SetOuter(CCoreMinimalObject* InOuter) { Outer = InOuter; }
	CCoreMinimalObject* GetOuter() {return Outer;}

public:
	// 这几个函数的实现是在FFrame.cpp文件中
	//static void Script_Undefined(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
	FUNCTION_DEFINITION(Script_Undefined);
	//static void Script_Int(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
	FUNCTION_DEFINITION(Script_Int);
	//static void Script_Float(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
	FUNCTION_DEFINITION(Script_Float);
	//static void Script_Function(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
	FUNCTION_DEFINITION(Script_Function);
protected:
	/** Looks for a given function name */
	UFunction* FindFunctionByName(std::string InName) const;
	static UFunction* StaticFindFunctionByName(std::string InName) { return nullptr; };
	/** Internal VM method for executing a function (ue no use staic)*/
	static void CallFunction(FFrame& Stack, void const* Data, UFunction* Function);
	/** Called by VM to execute a UFunction with a filled in UStruct of parameters */
	static void ProcessEvent(UFunction* Function, void* Parms);


public:
	FNativeClass& GetNativeClass() {return NativeClass;}
protected:
	virtual void InitReflectionContent() {};

private:
	///** Map of all functions by name contained in this class */
	//std::map<std::string, UFunction*> FunctionMap;
protected:
	FNativeClass NativeClass;

protected:
	bool isCanTick;
	CCoreMinimalObject* Outer; // 当前这个对象的上层对象
};

extern LURENJIACOREOBJECT_API vector<CCoreMinimalObject*> ObjectPool;
#pragma warning(pop)