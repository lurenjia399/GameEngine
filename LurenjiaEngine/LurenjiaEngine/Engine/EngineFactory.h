#pragma once
#include "Core/Engine.h"

class FEngineFactory
{
public:
	FEngineFactory();
	static shared_ptr<CEngine> CreateEngine();
};