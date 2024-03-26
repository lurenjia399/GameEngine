#include "CollectClassInfo.h"

bool CollectClassInfo::GetCodeTypeByFunc(const string& RowString, FFunctionAnalysis& OutFunctionAnalysis)
{
	char* ptr = const_cast<char*>(RowString.c_str());

	char L[1024] = { 0 };
	char R[1024] = { 0 };

	// UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	helper_tool_files::split(ptr, CodeTypeString, L, R, false);
	// ��벿��L ---- UFUNCTION(Meta = (
	// �Ұ벿��R ----  = "Describe", Group = "SimpleCodeLibrary"))
	helper_tool_files::trim_start_inline(R); // ȥ�����ܵĿո�
	// ��벿�ֲ�Ҫ�ˣ��ұ߲��� ---- = "Describe", Group = "SimpleCodeLibrary")) ȥ��һ���ո�
	vector<string> Emements = {};
	helper_tool_files::parse_into_vector_array(R, Emements, CommaString);
	// Emements = {"Describe",  Group = "SimpleCodeLibrary")}

	// �洢Function��CodeType
	if (!Emements.empty())
	{
		if (Emements[0].find("Event") != -1)
		{

			OutFunctionAnalysis.CodeType = "Event";
			return true;
		}
		else if (Emements[0].find("Describe") != -1)
		{
			OutFunctionAnalysis.CodeType = "Describe";
			return true;
		}
	}
	return false;
}

FParamElement CollectClassInfo::CollectionVariableType(const char* RowString, ECollectionParamType Type)
{
	FParamElement ReturnParam = {};
	char* ptr = (char*)RowString;
	
	//ReturnParam.Name  = "ReturnValue";

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
		// ע�������ȷ����������ֵ���͵�ʱ��const�о��ϲ���Ч�����ܵú����޸���
		helper_tool_files::trim_start_and_end_inline(ptr); // ȥ��ǰ��ո�
		helper_tool_files::remove_string_start(ptr, "const");
		ReturnParam.bConst = true;
	}
	// void test
	helper_tool_files::trim_start_and_end_inline(ptr); // ȥ��ǰ��ո�
	// void test
	if (Type == ECollectionParamType::TYPE_PARAM)
	{
		vector<string> Elements = {};
		helper_tool_files::parse_into_vector_array(ptr, Elements, SpaceString);
		// Elements = {"void", "test"}
		if (Elements.size() > 1)
		{
			ReturnParam.Type = (char*)Elements[0].c_str();
			ReturnParam.Name = Elements[1];
		}
	}
	else if (Type == ECollectionParamType::TYPE_RETURN)
	{
		helper_tool_files::remove_char_start(ptr, '\t');
		ReturnParam.Type = ptr;
		ReturnParam.Name = "ReturnValue";
		
	}
	return ReturnParam;
}

string CollectClassInfo::RemoveVariableDefaultValue(const char* RowString)
{
	char* ptr = (char*)RowString;

	// float FPS = 20.f;
	char LEqual[1024] = { 0 };
	char REqual[1024] = { 0 };
	helper_tool_files::split(ptr, EqualString, LEqual, REqual, false);
	// ��벿��L ----  float FPS 
	// �Ұ벿��R ----  20.f;
	{// ������벿��
		helper_tool_files::trim_start_and_end_inline(LEqual);
	}
	{// �Ұ벿�ֲ�Ҫ��
	}
	helper_tool_files::trim_start_inline(LEqual);

	return LEqual;
}

bool CollectClassInfo::GetCodeTypeByVariable(const string& RowString, FVariableAnalysis& OutVariableAnalysis)
{
	
	char* ptr = const_cast<char*>(RowString.c_str());

	char L[1024] = { 0 };
	char R[1024] = { 0 };

	// UPROPERTY(Meta = (CodeType = "Resources", Group = "simple"))
	helper_tool_files::split(ptr, CodeTypeString, L, R, false);
	// ��벿��L ---- UPROPERTY(Meta = (
	// �Ұ벿��R ----  = "Resources", Group = "simple"))
	vector<string> Elements = {};
	helper_tool_files::parse_into_vector_array(R, Elements, CommaString);
	// Elements = { = "Resources", "  Group = "simple"))"}
	if (!Elements.empty())
	{
		if (Elements[0].find("Resources"))
		{
			OutVariableAnalysis.CodeType = "Resources";
			return true;
		}
	}


	return false;
}

bool CollectClassInfo::Collection(const string& Paths, FClassAnalysis& OutClassAnalysis)
{
	vector<string> StringArray = {};
	helper_tool_files::load_file_to_strings(Paths, StringArray);
	//simple_cpp_helper_file::load_file_to_strings(Paths, StringArray);

	for (int i = 0; i < StringArray.size(); ++i)
	{
		string& row = StringArray[i];
		char* row_ptr = const_cast<char*>(row.c_str());//ÿһ�е��ַ�ָ��

		std::function<bool(const char*)> containFunc = [&](const char* InString) -> bool
			{
				// ����ط�������ǰ�ֵ����
				// �����ߵ����о��Ѿ�������
				// ���lambda���ʽ����޸�row������Ӱ��lambda��row��ֵ
				return row.find(InString) != -1;
			};

		// �Ƴ�ע�͵Ĵ���
		if(containFunc("//")) continue;



		if (containFunc("GENERATED_BODY"))
		{
			OutClassAnalysis.CodeLine = i + 1; //�洢GENERATED_BODY���ڵ�����
		}
//****** ��ȡ�����ͼ̳���
		if (containFunc("UCLASS") || containFunc("\tUCLASS"))
		{
			row = StringArray[i + 1];
			if ((containFunc("\tclass") || containFunc("class"))
				&& containFunc(":")
				&& (containFunc("protected") || containFunc("private") || containFunc("public")))
			{
				//class XX_API AActor : public CCoreMinimalObject
				helper_tool_files::remove_string_start(const_cast<char*>(row.c_str()), "class ");
				helper_tool_files::remove_string_start(const_cast<char*>(row.c_str()), "\tclass ");
				//XX_API AActor : public CCoreMinimalObject
				if (containFunc("_API"))
				{
					helper_tool_files::trim_start_inline(const_cast<char*>(row.c_str())); // ȥ�����ܵĿո�

					char L[1024] = { 0 };
					char R[1024] = { 0 };

					helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
					// ��벿��L ---- XX_API
					// �Ұ벿��R ---- AActor : public CCoreMinimalObject

					OutClassAnalysis.APIName = L;// �洢API

					row = R;
				}
				//AActor : public CCoreMinimalObject
				vector<string> Emements = {};
				helper_tool_files::parse_into_vector_array(const_cast<char*>(row.c_str()), Emements, ColonString);
				// Emements = {"AActor", "public CCoreMinimalObject"}
				helper_tool_files::trim_start_and_end_inline(const_cast<char*>(Emements[0].c_str()));//ȥ��ǰ��ո�
				OutClassAnalysis.ClassName = Emements[0]; // �洢����

				// ����Ҫ���Ƕ�̳е�����
				//Emements = { "CLightComponent", "public CTransformationComponent, public IDirectXDeviceInterface" }
				if (Emements.size() >= 2)
				{
					vector<std::string> InheritElements = {};
					helper_tool_files::parse_into_vector_array(Emements[1].c_str(), InheritElements, CommaString);
					// InheritElements = {""public CTransformationComponent", "public IDirectXDeviceInterface"}
					for (string& InheritElement : InheritElements)
					{
						char* temp = const_cast<char*>(InheritElement.c_str());
						helper_tool_files::trim_start_inline(temp); // ȥ��ǰ�߿ո�
						helper_tool_files::remove_string_start(temp, "public");
						helper_tool_files::remove_string_start(temp, "protected");
						helper_tool_files::remove_string_start(temp, "private");
						helper_tool_files::trim_start_inline(temp); // ȥ��ǰ�߿ո�

						OutClassAnalysis.InheritName.push_back(temp);//�洢�̳��ߵ�����
					}
				}
			}
		}
		

//****** ��ȡ������
		if (containFunc("UFUNCTION") || containFunc("\tUFUNCTION"))
		{
			FFunctionAnalysis FunctionAnalysis = {};
			GetCodeTypeByFunc(row, FunctionAnalysis);
			{
				bool bIsConstReturnParam = false;
				// UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
				row = StringArray[i + 1];//�õ�UFUNCTION����һ��
				// static void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				if (containFunc("\tstatic") || containFunc("static "))
				{
					// static void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
					FunctionAnalysis.bStatic = true;//�洢�Ǿ�̬����

					char L[1024] = { 0 };
					char R[1024] = { 0 };
					helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
					// ��벿��L ---- static
					// �Ұ벿��R ---- void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
					row = R;
					// void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				}
				else if(containFunc("\tvirtual") || containFunc("virtual "))
				{
					//virtual void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
					FunctionAnalysis.bVirtual = true;//�洢���麯��

					char L[1024] = { 0 };
					char R[1024] = { 0 };
					helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
					// ��벿��L ---- virtual
					// �Ұ벿��R ---- void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
					row = R;
					// void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				}
				else if (containFunc("\tconst") || containFunc("const "))
				{
					//const UAzureUI3DUserWidget* GetPateWidget();
					bIsConstReturnParam = true;

					char L[1024] = { 0 };
					char R[1024] = { 0 };
					helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
					// ��벿��L ---- const
					// �Ұ벿��R ---- UAzureUI3DUserWidget* GetPateWidget();
					row = R;
				}
				// void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				helper_tool_files::trim_start_inline(const_cast<char*>(row.c_str()));
				char L[1024] = { 0 };
				char R[1024] = { 0 };
				helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
				// ��벿��L ---- void
				// �Ұ벿��R ---- lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo) {};
				{// ������벿��
					
					FunctionAnalysis.ReturnParam = CollectionVariableType(L, ECollectionParamType::TYPE_RETURN);//�洢����ֵ����
					FunctionAnalysis.ReturnParam.bConst = bIsConstReturnParam;//�洢����ֵ����
				}
				{// �����Ұ벿��
					// �Ұ벿��R ---- lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo) {};
					helper_tool_files::remove_char_end(R, '}');
					helper_tool_files::remove_char_end(R, '{');
					helper_tool_files::trim_end_inline(R);
					helper_tool_files::remove_char_end(R, ';');
					helper_tool_files::remove_char_end(R, ')');
					// lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo)
					char L_halfradius[1024] = { 0 };
					char R_halfradius[1024] = { 0 };
					helper_tool_files::split(R, LeftHalfRadius, L_halfradius, R_halfradius, false);
					// ��벿��L_halfradius ---- lurenjiaTestFunction
					// �Ұ벿��R_halfradius ---- float DeltaTime, const FViewportInfo& ViewportInfo)
					FunctionAnalysis.FunctionName = L_halfradius;//�洢��������
					// �Ұ벿��R_halfradius ---- float DeltaTime, const FViewportInfo& ViewportInfo)
					vector<string> Elements = {};
					helper_tool_files::parse_into_vector_array(R_halfradius, Elements, CommaString);
					// Emements = {"float DeltaTime", "const FViewportInfo& ViewportInfo"}
					for (string& element : Elements)
					{
						FParamElement param = CollectionVariableType(element.c_str(), ECollectionParamType::TYPE_PARAM);//�洢����ֵ����
						FunctionAnalysis.ParamArray.push_back(param);//�洢��������
					}
					OutClassAnalysis.Functions.push_back(FunctionAnalysis);//�洢��ĺ���
				}
			}
		}

//****** ��ȡ��Ա����
		if (containFunc("UPROPERTY") || containFunc("\tUPROPERTY"))
		{
			FVariableAnalysis VariableAnalysis = {};
			GetCodeTypeByVariable(row, VariableAnalysis);
			{
				bool bIsConstVariableParam = false;
				row = StringArray[i + 1];
				// �ı�rowΪUproperty���ε���һ��
				// AActor* actor;
				helper_tool_files::remove_char_start(const_cast<char*>(row.c_str()), '\t');
				helper_tool_files::remove_char_end(const_cast<char*>(row.c_str()), ';');
				// const AActor* actor
				char L[1024] = { 0 };
				char R[1024] = { 0 };
				string Name = "";
				if (containFunc("const"))
				{
					//const float FPS = 20.f;
					bIsConstVariableParam = true;
					helper_tool_files::remove_string_start(const_cast<char*>(row.c_str()), "const");
					if (containFunc(EqualString))
					{
						row = RemoveVariableDefaultValue(const_cast<char*>(row.c_str()));
					}
				}
				if (containFunc(StarString))
				{
					VariableAnalysis.bPointer = true;
					helper_tool_files::split(const_cast<char*>(row.c_str()), StarString, L, R, false);
					if (containFunc(EqualString))
					{
						string temp = RemoveVariableDefaultValue(R);
						strncpy_s(R, temp.size() + 1, temp.c_str(), temp.size());
					}

				}else if (containFunc(ReferenceString))
				{
					//float& FPS_reference = 20.f;
					VariableAnalysis.bReference = true;
					helper_tool_files::split(const_cast<char*>(row.c_str()), ReferenceString, L, R, false);

					if (containFunc(EqualString))
					{
						string temp = RemoveVariableDefaultValue(R);
						strncpy_s(R, temp.size() + 1, temp.c_str(), temp.size());
					}
				}else if (containFunc(SpaceString))
				{
					helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
					if (containFunc(EqualString))
					{
						string temp = RemoveVariableDefaultValue(R);
						strncpy_s(R, temp.size() + 1, temp.c_str(), temp.size());
					}
				}
				else {
					Name = "nullptr";
				}

				helper_tool_files::trim_start_and_end_inline(const_cast<char*>(row.c_str()));

				VariableAnalysis.bConst = bIsConstVariableParam;
				VariableAnalysis.Type = L;
				VariableAnalysis.Name = R;
				OutClassAnalysis.Variable.push_back(VariableAnalysis);
			}
		}

	}



	return false;
}