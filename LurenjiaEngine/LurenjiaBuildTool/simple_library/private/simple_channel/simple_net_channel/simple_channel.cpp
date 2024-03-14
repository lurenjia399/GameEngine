// Copyright (C) RenZhai.2022.All Rights Reserved.
#include "../../../../simple_library/public/simple_channel/simple_core/simple_channel.h"
#include "../../../../simple_library/public/simple_channel/simple_core/simple_connetion.h"
//#include "../../../../simple_library/public/simple_math/simple_math.h"
#include "../../../../simple_library/private/simple_math/simple_math.c"

FSimpleChannel::FSimpleChannel()
	:Connetion(nullptr)
	,ID(get_uint32_random(9999999))
{

}

void FSimpleChannel::Send(TArray<unsigned char>& InBuffer)
{
	Connetion->SetBuffer(InBuffer);
	Connetion->Send();
}

bool FSimpleChannel::Receive(TArray<unsigned char>& InBuffer)
{
	Connetion->RecvBuffer(InBuffer);
	return true;
}

void FSimpleChannel::SetConnetion(FSimpleConnetion* InConnetion)
{
	Connetion = InConnetion;
}

void FSimpleChannel::SetGuid(unsigned int InGuid)
{
	ID = InGuid;
}