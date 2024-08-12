#include "LurenjiaBuildTool.h"
#include "CodeReflection/Collect/CollectClassInfo.h"
#include "CodeReflection/IntermediateFile/IntermediateFile.h"


int main()
{
	// 测试用
	{
		//char path[] = "D:/GitHub/GameEngine/LurenjiaEngine/LurenjiaEngine/Engine/Test/CodeReflection/CodeReflectionTest.h";
		//char h_path[] = "D:/GitHub/GameEngine/LurenjiaEngine/LurenjiaBuildTool/Code/TestObject.generate.h";
		//char cpp_path[] = "D:/GitHub/GameEngine/LurenjiaEngine/LurenjiaBuildTool/Code/TestObject.gen.cpp";

		//FClassAnalysis ClassAnalysis = {};
		//CollectClassInfo::Collection(path, ClassAnalysis);

		//vector<string> OutAnalysisRawH;
		//vector<string> OutAnalysisRawCPP;

		//IntermediateFile::Builder(ClassAnalysis, OutAnalysisRawH, OutAnalysisRawCPP);

		//helper_tool_files::save_file_to_strings(h_path, OutAnalysisRawH);
		//helper_tool_files::save_file_to_strings(cpp_path, OutAnalysisRawCPP);
	}


	string CodeReflectionPath = FEnginePathHelper::GetEngineCodeReflectionPath();
	string SourcePath = FEnginePathHelper::GetEngineSourcePath();

	// 移除反射路径下的文件
	remove_dir_all_files(CodeReflectionPath.c_str());
	// 找到Source路径下的文件
	def_c_paths Paths;
	init_def_c_paths(&Paths);
	find_files(SourcePath.c_str(), &Paths, true, true);

	// 开始遍历
	for (int i = 0; i < Paths.index; i++)
	{
		// 找到代码中的.h文件
		if (helper_tool_files::find_string(Paths.paths[i], ".h", 0) != -1)
		{
			helper_tool_files::normalization_path(Paths.paths[i]);

			// 读取文件到StringArray数组中
			std::vector<std::string> StringArray;
			helper_tool_files::load_file_to_strings(Paths.paths[i], StringArray);

			// 判断是否可以生成反射代码
			if (bIsCheckAllowCodeReflection(StringArray))
			{
				// 收集类信息
				FClassAnalysis ClassAnalysis;
				CollectClassInfo::Collection(Paths.paths[i], ClassAnalysis);
			
				// .generated.h文件
				vector<string> OutAnalysisRawH;
				// .gen.cpp文件
				vector<string> OutAnalysisRawCPP;
				// 生成.generated.h文件和.gen.cpp文件
				IntermediateFile::Builder(ClassAnalysis, OutAnalysisRawH, OutAnalysisRawCPP);
				
				// 拿到路径中的文件名称xxx.h
				char Buff[1024] = { 0 };
				get_path_clean_filename(Buff, Paths.paths[i]);
				// 移除.h得到文件名称
				helper_tool_files::remove_char_end(Buff, 'h');
				helper_tool_files::remove_char_end(Buff, '.');

				// 将.generate.h和.gen.cpp文件保存到相应的位置上去
				string h_path = CodeReflectionPath + "/" + Buff + ".generated.h";
				string cpp_path = CodeReflectionPath + "/" + Buff + ".gen.cpp";
				simple_cpp_helper_file::save_file_to_strings(h_path, OutAnalysisRawH);
				simple_cpp_helper_file::save_file_to_strings(cpp_path, OutAnalysisRawCPP);
			}
		}
	}


	return 0;
}