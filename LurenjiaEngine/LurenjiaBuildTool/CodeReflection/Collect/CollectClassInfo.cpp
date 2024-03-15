#include "CollectClassInfo.h"

bool CollectClassInfo::GetCodeTypeByFunc(const string& RowString, FFunctionAnalysis& FunctionAnalysis)
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
		char* row_ptr = const_cast<char*>(row.c_str());//ÿһ�е��ַ�ָ��

		std::function<bool(const char*)> containFunc = [row](const char* InString) -> bool
			{
				// ����ط��ǰ�ֵ����
				// �����ߵ����о��Ѿ�������
				// ���lambda���ʽ����޸�row������Ӱ��lambda��row��ֵ
				return row.find(InString) != -1;
			};

		
		if (containFunc("GENERATED_BODY"))
		{
			OutClassAnalysis.CodeLine = i + 1; //�洢GENERATED_BODY���ڵ�����
		}
//****** ��ȡ�����ͼ̳���
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
				helper_tool_files::trim_start_inline(row_ptr); // ȥ�����ܵĿո�

				char L[1024] = {0};
				char R[1024] = {0};

				helper_tool_files::split(row_ptr, SpaceString, L, R, false);
				// ��벿��L ---- XX_API
				// �Ұ벿��R ---- AActor : public CCoreMinimalObject

				OutClassAnalysis.APIName = L;// �洢API

				row = R;
			}
			//AActor : public CCoreMinimalObject
			vector<string> Emements = {};
			helper_tool_files::parse_into_vector_array(row_ptr, Emements, ColonString);
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
//****** ��ȡ������
		if (containFunc("UFUNCTION"))
		{
			FFunctionAnalysis FunctionAnalysis = {};
			if (GetCodeTypeByFunc(row, FunctionAnalysis))
			{
				// UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
				// static void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				row = StringArray[i + 1];//�õ�UFUNCTION����һ��
				// static void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				if (containFunc("\tstatic") || containFunc("static "))
				{
					FunctionAnalysis.bStatic = true;//�洢�Ƿ��Ǿ�̬����

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
					FunctionAnalysis.bVirtual = true;//�洢�Ƿ����麯��

					char L[1024] = { 0 };
					char R[1024] = { 0 };
					helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
					// ��벿��L ---- virtual
					// �Ұ벿��R ---- void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
					row = R;
					// void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				}
				// void lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo);
				helper_tool_files::trim_start_inline(const_cast<char*>(row.c_str()));
				char L[1024] = { 0 };
				char R[1024] = { 0 };
				helper_tool_files::split(const_cast<char*>(row.c_str()), SpaceString, L, R, false);
				// ��벿��L ---- void
				// �Ұ벿��R ---- lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo) {};
				FunctionAnalysis.ReturnParam = CollectionVariableType(L, ECollectionParamType::TYPE_RETURN);//�洢����ֵ����
				// �Ұ벿��R ---- lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo) {};
				helper_tool_files::remove_char_end(R, '}');
				helper_tool_files::remove_char_end(R, '{');
				helper_tool_files::trim_end_inline(R);
				helper_tool_files::remove_char_end(R, ';');
				helper_tool_files::remove_char_end(R, ')');
				// lurenjiaTestFunction(float DeltaTime, const FViewportInfo& ViewportInfo)
				char L_halfradius[1024] = { 0 };
				char R_halfradius[1024] = { 0 };
				helper_tool_files::split(const_cast<char*>(row.c_str()), LeftHalfRadius, L_halfradius, R_halfradius, false);
				// ��벿��L_halfradius ---- lurenjiaTestFunction
				// �Ұ벿��R_halfradius ---- float DeltaTime, const FViewportInfo& ViewportInfo)
				FunctionAnalysis.FunctionName = L_halfradius;//�洢��������
				// �Ұ벿��R_halfradius ---- float DeltaTime, const FViewportInfo& ViewportInfo)
				vector<string> Elements = {};
				helper_tool_files::parse_into_vector_array(R_halfradius, Elements, CommaString);
				// Emements = {"float DeltaTime", "const FViewportInfo& ViewportInfo"}
				for (string& element : Elements)
				{
					FParamElement param = CollectionVariableType(element, ECollectionParamType::TYPE_PARAM);//�洢����ֵ����
					FunctionAnalysis.ParamArray.push_back(param);//�洢��������
				}
				OutClassAnalysis.Functions.push_back(FunctionAnalysis);//�洢��ĺ���
			}
		}

	}



	return false;
}