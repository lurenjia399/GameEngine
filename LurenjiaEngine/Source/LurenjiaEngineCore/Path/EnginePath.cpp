#include "EnginePath.h"

std::string FEnginePathHelper::GetEngineRootPath()
{
    return std::string();
}

std::string FEnginePathHelper::GetEngineLogsPath()
{

	const char Path[] = "../../Saved/Logs";

	string result = GetAbsPath(Path);
	create_file_directory(result.c_str());

    return result;
}

string FEnginePathHelper::GetEngineContentPath()
{
	const char Path[] = "../../Content";

	string result = GetAbsPath(Path);
	create_file_directory(result.c_str());

	return result;
}

string FEnginePathHelper::GetAbsPath(const string& Inpath)
{
	char path[1024] = { 0 };
	get_full_path(path, 1024, Inpath.c_str());

	return path;
}
