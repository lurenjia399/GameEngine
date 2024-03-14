#include "CollectClassInfo.h"

bool CollectClassInfo::Collection(const string& Paths, FClassAnalysis& OutClassAnalysis)
{
	vector<string> StringArray;
	helper_tool_files::load_file_to_strings(Paths, StringArray);

	for (int i = 0; i < StringArray.size(); ++i)
	{
		string& row = StringArray[i];
		char* row_ptr = const_cast<char*>(row.c_str());//每一行的字符指针

		std::function<bool(const char*)> containFunc = [row](const char* InString) -> bool
			{
				// 这个地方是按值捕获
				// 代码走到这行就已经捕获了
				// 如果lambda表达式后边修改row，不会影响lambda里row的值
				return row.find(InString) != -1;
			};

		// 存储GENERATED_BODY所在的行数
		if (containFunc("GENERATED_BODY"))
		{
			OutClassAnalysis.CodeLine = i + 1; 
		}
		// 获取类名和继承名
		if ((containFunc("\tclass") || containFunc("class")) 
			&& containFunc(":") 
			&& (containFunc("protected") || containFunc("private") || containFunc("public")))
		{
			//class XX_API AActor : public CCoreMinimalObject
			helper_tool_files::remove_string_start(row_ptr, "class ");
			helper_tool_files::remove_string_start(row_ptr, "\tclass ");
			//XX_API AActor : public CCoreMinimalObject
			if (containFunc("_API"))
			{
				helper_tool_files::trim_start_inline(row_ptr); // 去掉可能的空格

				char L[1024] = {0};
				char R[1024] = {0};

				helper_tool_files::split(row_ptr, SpaceString, L, R, false);
				// 左半部分L ---- XX_API
				// 右半部分R ---- AActor : public CCoreMinimalObject

				OutClassAnalysis.APIName = L;// 存储API

				row = R;
			}
			//AActor : public CCoreMinimalObject
			vector<string> Emements = {};
			helper_tool_files::parse_into_vector_array(row_ptr, Emements, ColonString);
			// Emements = {"AActor", "public CCoreMinimalObject"}
			helper_tool_files::trim_start_and_end_inline(const_cast<char*>(Emements[0].c_str()));//去除前后空格
			OutClassAnalysis.ClassName = Emements[0]; // 存储类名

			// 还需要考虑多继承的问题
			//Emements = { "CLightComponent", "public CTransformationComponent, public IDirectXDeviceInterface" }
			if (Emements.size() >= 2)
			{
				vector<std::string> InheritElements = {};
				helper_tool_files::parse_into_vector_array(Emements[1].c_str(), InheritElements, CommaString);
				// InheritElements = {""public CTransformationComponent", "public IDirectXDeviceInterface"}
				for (string& InheritElement : InheritElements)
				{
					char* temp = const_cast<char*>(InheritElement.c_str());
					helper_tool_files::trim_start_inline(temp); // 去除前边空格
					helper_tool_files::remove_string_start(temp, "public");
					helper_tool_files::remove_string_start(temp, "protected");
					helper_tool_files::remove_string_start(temp, "private");
					helper_tool_files::trim_start_inline(temp); // 去除前边空格

					OutClassAnalysis.InheritName.push_back(temp);//存储继承者的名字
				}
			}

		}

	}



	return false;
}