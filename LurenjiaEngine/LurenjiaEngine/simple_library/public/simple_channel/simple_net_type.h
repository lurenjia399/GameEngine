// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "simple_library/public/simple_core_minimal/simple_c_core/simple_core_minimal.h"
#include "simple_library/public/simple_library_macro.h"

enum class ESimpleSocketType :unsigned char
{
	SOCKETTYPE_UDP,
	SOCKETTYPE_TCP,
}; 

enum class ESimpleDriveType :unsigned char
{
	DRIVETYPE_LISTEN,//服务器
	DRIVETYPE_CONNECTION,
};

enum class ESimpleConnetionState :unsigned char
{
	FREE,
	VERSION_VERIFICATION,
	LOGIN,
	JOIN,
};

enum class ESimpleConnetionType :unsigned char
{
	CONNETION_MAIN_LISTEN, //主通道 
	CONNETION_LISTEN,	  //监听已经投递过来的
};

struct SIMPLE_LIBRARY_API FSimpleIOData
{
	FSimpleIOData();

	OVERLAPPED Overlapped;
	CHAR Buffer[1024];
	BYTE Type;
	DWORD Len;
	WSABUF WsaBuffer;
};

struct SIMPLE_LIBRARY_API FSimpleBunchHead
{
	FSimpleBunchHead();

	unsigned int Protocols;
	unsigned int ChannelID;
	unsigned int ParamNum;
};