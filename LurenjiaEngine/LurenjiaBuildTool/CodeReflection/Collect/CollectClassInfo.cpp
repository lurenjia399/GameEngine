#include "CollectClassInfo.h"

bool CollectClassInfo::GetCodeTypeByFunc(const string& RowString, FFunctionAnalysis& FunctionAnalysis)
{
	char* ptr = const_cast<char*>(RowString.c_str());

	char L[1024] = { 0 };
	char R[1024] = { 0 };

	// UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	helper_tool_files::split(ptr, CodeTypeString, L, R, false);
	// 左半部分L ---- UFUNCTION(Meta = (
	// 右半部分R ----  = "Describe", Group = "SimpleCodeLibrary"))
	helper_tool_files::trim_start_inline(R); // 去掉可能的空格
	// 左半部分不要了，右边部分 ---- = "Describe", Group = "SimpleCodeLibrary")) 去掉一个空格
	vector<string> Emements = {};
	helper_tool_files::parse_into_vector_array(R, Emements, CommaString);
	// Emements = {"Describe",  Group = "SimpleCodeLibrary")}

	// 存储Function的CodeType
	if (!Emements.empty())
	{
		if (Emements[0].find("Event"))
		{
			FunctionAnalysis.CodeType = "Event";
			return true;
		}
		else if (Emements[0].find("Describe"))
		{
			FunctionAnalysis.CodeType = "Describe";
			return true;
		}
	}
	return false;
}

FParamElement CollectClassInfo::CollectionVariableType(const string& RowString, ECollectionParamType Type)
{
	FParamElement ReturnParam = {};
	char* ptr = const_cast<char*>(RowString.c_str());
	
	ReturnParam.Name  = "ReturnValue";

	// const void* test
	if (helper_tool_files::string_contain(ptr, StarString))
	{
		helper_tool_files::remove_char_end(ptr, '*');
		ReturnParam.bPointer = true;
	}
	// const void test
	if (helper_tool_files::string_contain(ptr, ReferenceString))
	{
		helper_tool_files::remove_char_end(ptr, '&');
		ReturnParam.bReference = true;
	}
	// const void test
	if (helper_tool_files::string_contain(ptr, "const"))
	{
		// 注意这里，在确定函数返回值类型的时候，const感觉上不生效，可能得后续修改下
		helper_tool_files::trim_start_and_end_inline(ptr); // 去掉前后空格
		helper_tool_files::remove_string_start(ptr, "const");
		ReturnParam.bConst = true;
	}
	// void test
	helper_tool_files::trim_start_and_end_inline(ptr); // 去掉前后空格
	// void test
	if (Type == ECollectionParamType::TYPE_PARAM)
	{
		vector<string> Elements = {};
		helper_tool_files::parse_into_vector_array(ptr, Elements, SpaceString);
		// Elements = {"void", "test"}
		if (Elements.size() > 1)
		{
			ptr = const_cast<char*>(Elements[0].c_str());
			ReturnParam.Name = Elements[1];
		}
	}
	else if (Type == ECollectionParamType::TYPE_RETURN)
	{
		ReturnParam.Name = "null";
	}

	ReturnParam.Type = ptr;


	return ReturnParam;
}

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

		
		if (containFunc("GENERATED_BODY"))
		{
			OutClassAnalysis.CodeLine = i + 1; //存储GENERATED_BODY所在的行数
		}
//****** 获取类名和继承名
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
//****** 获取函数名
		if (containFunc("UFUNCTION"))
		{
			FFunctionAnalysis FunctionAnalysis = {};
			if (GetCodeTypeByFunc(row, FunctionAnalysis))
			{
				// UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
				// static void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				row = StringArray[i + 1];//拿到UFUNCTION的下一行
				// static void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				if (containFunc("\tstatic") || containFunc("static "))
				{
					FunctionAnalysis.bStatic = true;//存储是否是静态方法

					char L[1024] = { 0 };
					char R[1024] = { 0 };
					helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
					// 左半部分L ---- static
					// 右半部分R ---- void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
					row = R;
					// void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				}
				else if(containFunc("\tvirtual") || containFunc("virtual "))
				{
					FunctionAnalysis.bVirtual = true;//存储是否是虚函数

					char L[1024] = { 0 };
					char R[1024] = { 0 };
					helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
					// 左半部分L ---- virtual
					// 右半部分R ---- void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
					row = R;
					// void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				}
				// void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				helper_tool_files::trim_start_inline(const_cast<char*>(row.c_str()));
				char L[1024] = { 0 };
				char R[1024] = { 0 };
				helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
				// 左半部分L ---- void
				// 右半部分R ---- lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo) {};
				FunctionAnalysis.ReturnParam = CollectionVariableType(L, ECollectionParamType::TYPE_RETURN);//存储返回值参数
				// 右半部分R ---- lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo) {};
				helper_tool_files::remove_char_end(R, '}');
				helper_tool_files::remove_char_end(R, '{');
				helper_tool_files::trim_end_inline(R);
				helper_tool_files::remove_char_end(R, ';');
				helper_tool_files::remove_char_end(R, ')');
				// lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo)
				char L_halfradius[1024] = { 0 };
				char R_halfradius[1024] = { 0 };
				helper_tool_files::split(const_cast<char*>(row.c_str()), LeftHalfRadius, L_halfradius, R_halfradius, false);
				// 左半部分L_halfradius ---- lurenjiaTestFunction
				// 右半部分R_halfradius ---- float DeltaTime, const FViewportInfo& ViewportInfo)
				FunctionAnalysis.FunctionName = L_halfradius;//存储函数名称
				// 右半部分R_halfradius ---- float DeltaTime, const FViewportInfo& ViewportInfo)
				vector<string> Elements = {};
				helper_tool_files::parse_into_vector_array(R_halfradius, Elements, CommaString);
				// Emements = {"float DeltaTime", "const FViewportInfo& ViewportInfo"}
				for (string& element : Elements)
				{
					FParamElement param = CollectionVariableType(element, ECollectionParamType::TYPE_PARAM);//存储返回值参数
					FunctionAnalysis.ParamArray.push_back(param);//存储参数数组
				}
				OutClassAnalysis.Functions.push_back(FunctionAnalysis);//存储类的函数
			}
		}

	}



	return false;
}