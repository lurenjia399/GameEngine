#include "EngineFactory.h"
#if defined(_WIN32)
	#include "Platform/Windows/WindowsEngine.h"
#endif

FEngineFactory::FEngineFactory()
{
}

CEngine* FEngineFactory::CreateEngine()
{
#if defined(_WIN32)
	 CWindowsEngine * wEngine = new CWindowsEngine();
	 wEngine->ResetGuid("wEngine");
	 return wEngine;
#elif defined(__linux__)

#endif
}