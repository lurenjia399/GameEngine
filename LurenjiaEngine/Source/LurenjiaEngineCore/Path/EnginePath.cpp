#include "EnginePath.h"

std::string FEnginePathHelper::GetEngineRootPath()
{
    return std::string();
}

std::string FEnginePathHelper::GetEngineBinariesPath()
{
	const char Path[] = "../../Binaries/Win64";

	string result = GetAbsPath(Path);

	return result;
}

std::string FEnginePathHelper::GetEngineLogsPath()
{

	const char Path[] = "../../Saved/Logs";

	string result = GetAbsPath(Path);

    return result;
}

string FEnginePathHelper::GetEngineContentPath()
{
	const char Path[] = "../../Content";

	string result = GetAbsPath(Path);

	return result;
}

string FEnginePathHelper::GetEngineShadersPath()
{
	const char Path[] = "../../Shaders";

	string result = GetAbsPath(Path);

	return result;
}

wstring FEnginePathHelper::GetEngineShadersWidthPath()
{
	const char Path[] = "../../Shaders";

	string result = GetAbsPath(Path);

	return s2ws(result);
}

wstring FEnginePathHelper::s2ws(const string& str)
{
	_bstr_t t = str.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

string FEnginePathHelper::GetAbsPath(const string& Inpath)
{
	char path[1024] = { 0 };
	get_full_path(path, 1024, Inpath.c_str());

	return path;
}
