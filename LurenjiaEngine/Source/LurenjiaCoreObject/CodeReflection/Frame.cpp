#include "Frame.h"
#include "ScriptCommand.h"


FNativeFuntionPtr GScriptNatives[Script_Max];

FOutParam::FOutParam()
	: PropertyAddr(nullptr)
{
}


FFrame::FFrame()
{
}

FFrame::FFrame(UFunction* InNewFunction)
{
}

float FFrame::ReadFloat()
{
	return Read<int>();
}

int FFrame::ReadInt()
{
	return Read<int>();
}

void FFrame::Step(CCoreMinimalObject* Context, void const* RefData)
{
	int B = *Code++;
	(GScriptNatives[B])(Context, this, RefData);
}

unsigned char* FFrame::GetParamAddr()
{
	UProperty* Property = OutParam->PropertyAddr;
	OutParam = OutParam->Next;

	return nullptr;
}

void FFrame::AddOutParam(UProperty* Addr)
{
	std::shared_ptr<FOutParam> ListParamFirst = OutParam;
	if (!ListParamFirst.get())
	{
		OutParam = std::make_shared<FOutParam>();
		OutParam->PropertyAddr = Addr;
	}
	else
	{
		while (ListParamFirst->Next.get())
		{
			ListParamFirst = ListParamFirst->Next;
		}
		ListParamFirst->Next->PropertyAddr = Addr;
	}
}

//void CCoreMinimalObject::Script_Undefined(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Undefined)
{

}

//void CCoreMinimalObject::Script_Int(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Int)
{
	*(int*)RefData = FrameStack->ReadInt();
}
//void CCoreMinimalObject::Script_Float(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Float)
{
	*(float*)RefData = FrameStack->ReadInt();
}
//void CCoreMinimalObject::Script_Function(CCoreMinimalObject* Context, FFrame* FrameStack, void const* RefData)
FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Function)
{

}

