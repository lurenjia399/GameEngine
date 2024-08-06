#pragma once

#include "../CoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../CoreObject/CoreMinimalObject.h"

class UFunction;
class UProperty;

class FOutParam :public std::enable_shared_from_this<FOutParam>
{
public:
	FOutParam();
	UProperty* PropertyAddr;//参数指针
	std::shared_ptr<FOutParam> Next;//指向下一个链表指针
};


struct FFrame
{
	FFrame();
	FFrame(UFunction* InNewFunction);

	// ---------- 读取 start
	template<typename type>
	type Read();
	float ReadFloat();
	int ReadInt();
	void Step(CCoreMinimalObject* Context, void const* RefData);
	LURENJIACOREOBJECT_API unsigned char* GetParamAddr();
	// ---------- 读取 end
	// 将Addr接入OutParam链表
	void AddOutParam(UProperty* Addr);

public:
	unsigned char* Code;//字节码的首地址
	UFunction* Function;//方法
	std::shared_ptr<FOutParam> OutParam;//参数

};

// 定义一个函数指针
typedef void(*FNativeFuntionPtr)(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData);

template<typename type>
type FFrame::Read()
{
	type Result = 0;
	int Len = sizeof(type);
	memcpy(&Result, Code, Len);

	Code += Len;

	return Result;
}
