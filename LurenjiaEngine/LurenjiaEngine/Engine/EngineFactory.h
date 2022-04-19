#pragma once
#include "Core/Engine.h"

class FEngineFactory
{
public:
	FEngineFactory();
	static CEngine* CreateEngine();
};