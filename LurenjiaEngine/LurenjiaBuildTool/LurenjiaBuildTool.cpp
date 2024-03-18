#include "simple_library/public/simple_library.h"
#include "CodeReflection/Collect/CollectClassInfo.h"
#include "CodeReflection/IntermediateFile/IntermediateFile.h"


int main()
{
	char path[] = "D:/GitHub/GameEngine/LurenjiaEngine/LurenjiaEngine/Engine/Test/CodeReflection/CodeReflectionTest.h";
	char h_path[] = "D:/GitHub/GameEngine/LurenjiaEngine/LurenjiaBuildTool/Code/TestObject.generate.h";
	char cpp_path[] = "D:/GitHub/GameEngine/LurenjiaEngine/LurenjiaBuildTool/Code/TestObject.gen.cpp";

	FClassAnalysis ClassAnalysis = {};
	CollectClassInfo::Collection(path, ClassAnalysis);

	vector<string> OutAnalysisRawH;
	vector<string> OutAnalysisRawCPP;

	IntermediateFile::Builder(ClassAnalysis, OutAnalysisRawH, OutAnalysisRawCPP);

	helper_tool_files::save_file_to_strings(h_path, OutAnalysisRawH);
	helper_tool_files::save_file_to_strings(cpp_path, OutAnalysisRawCPP);

	return 0;
}