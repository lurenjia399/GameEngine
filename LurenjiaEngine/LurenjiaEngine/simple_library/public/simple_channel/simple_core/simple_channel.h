// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once
#include "simple_library/public/simple_array/simple_hash_array.h"
#include "simple_library/public/simple_library_macro.h"

class FSimpleConnetion;
class SIMPLE_LIBRARY_API FSimpleChannel
{
public:
	FSimpleChannel();

	unsigned int GetGuid() { return ID; };

	void SetGuid(unsigned int InGuid);

	void Send(TArray<unsigned char> &InBuffer);
	bool Receive(TArray<unsigned char>& InBuffer);

	void SetConnetion(FSimpleConnetion* InConnetion);
protected:
	FSimpleConnetion* Connetion;
	unsigned int ID;
};