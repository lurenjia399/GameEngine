#pragma once
#include "../CoreObjectMinimal.h"

// 字节码表
enum EScriptCommand
{
	Script_Undefined = 0x0B,// ASCII对应垂直制表符
	Script_Int = 0x1D,//
	Script_Float = 0x1D,// ASCII对应分组符
	Script_Function = 0x77,// ASCII对应小写字母w


	Script_Max = 0x100,

};