#include "EngineFactory.h"
#if defined(_WIN32)
	#include "Platform/Windows/WindowsEngine.h"
#endif

FEngineFactory::FEngineFactory()
{
}

shared_ptr<CEngine> FEngineFactory::CreateEngine()
{
#if defined(_WIN32)
	 //CWindowsEngine * wEngine = new CWindowsEngine();
	 shared_ptr<CWindowsEngine> wEngine = make_shared<CWindowsEngine>();
	 wEngine->ResetGuid("wEngine");
	 //return static_pointer_cast<CEngine>(wEngine);
	 return wEngine;
#elif defined(__linux__)

#endif
}