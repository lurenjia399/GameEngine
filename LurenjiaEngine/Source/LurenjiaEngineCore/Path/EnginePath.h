#pragma once

#include "EngineCore.h"
#include "EngineCoreMacro.h"

// 再用动态库的时候，需要写这个宏导出
// 再用静态库的时候，不用这个这个宏
struct LURENJIAENGINE_API FEnginePathHelper
{
	static std::string GetEngineRootPath();

	static std::string GetEngineBinariesPath();

	static std::string GetEngineLogsPath();

	static string GetEngineContentPath();

	static string GetEngineContentFBXPath();

	static string GetEngineShadersPath();

	static wstring GetEngineShadersWidthPath();

	static wstring s2ws(const string& str);

	static string GetAbsPath(const string& Inpath);
};