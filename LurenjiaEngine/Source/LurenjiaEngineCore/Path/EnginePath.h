#pragma once

#include "EngineCore.h"
#include "EngineCoreMacro.h"

struct LURENJIAENGINE_API FEnginePathHelper
{
	static std::string GetEngineRootPath();

	static std::string GetEngineLogsPath();

	static string GetEngineContentPath();

	static string GetAbsPath(const string& Inpath);
};