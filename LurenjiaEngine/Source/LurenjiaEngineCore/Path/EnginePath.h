#pragma once

#include "EngineCore.h"
#include "EngineCoreMacro.h"

struct LURENJIAENGINE_API FEnginePathHelper
{
	static std::string GetEngineRootPath();

	static std::string GetEngineBinariesPath();

	static std::string GetEngineLogsPath();

	static string GetEngineContentPath();

	static string GetEngineShadersPath();

	static wstring GetEngineShadersWidthPath();

	static wstring s2ws(const string& str);

	static string GetAbsPath(const string& Inpath);
};