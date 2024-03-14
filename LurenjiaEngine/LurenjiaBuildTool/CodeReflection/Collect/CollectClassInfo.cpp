#include "CollectClassInfo.h"

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

		// �洢GENERATED_BODY���ڵ�����
		if (containFunc("GENERATED_BODY"))
		{
			OutClassAnalysis.CodeLine = i + 1; 
		}
		// ��ȡ�����ͼ̳���
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

	}



	return false;
}