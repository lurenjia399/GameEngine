#include "IntermediateFile.h"

bool IntermediateFile::Builder(const FClassAnalysis& InClassAnalysis, vector<string>& OutAnalysisRawH, vector<string>& OutAnalysisRawCPP)
{
    vector<string> StaticRegistration = {};
    GenerateFile_H(InClassAnalysis, StaticRegistration, OutAnalysisRawH);
    GenerateFile_CPP(InClassAnalysis, StaticRegistration, OutAnalysisRawCPP);

    return !OutAnalysisRawH.empty() && !OutAnalysisRawCPP.empty();
}

void IntermediateFile::GenerateFile_H(const FClassAnalysis& InClassAnalysis, vector<string>& StaticRegistration, vector<string>& OutAnalysisRaw)
{
    OutAnalysisRaw.push_back("#pragma once");
    OutAnalysisRaw.push_back("");
    OutAnalysisRaw.push_back("#include \"ObjectScript.h\"");
    OutAnalysisRaw.push_back("");

    std::string MyClassName = " Z_BT_" + InClassAnalysis.ClassName;// Z_BT_UParticleSystem

    OutAnalysisRaw.push_back(std::string("#define ") + MyClassName + (InClassAnalysis.Functions.size() > 0 ? " \\" : ""));

    std::string ClearClassName = InClassAnalysis.ClassName;
    {
        char* ClearClassNamePtr = (char*)ClearClassName.c_str();

        helper_tool_files::trim_start_and_end_inline(ClearClassNamePtr);
        helper_tool_files::remove_char_start(ClearClassNamePtr, 'A');
        helper_tool_files::remove_char_start(ClearClassNamePtr, 'C');
        //helper_tool_files::remove_char_start(ClearClassNamePtr, 'I');
        //helper_tool_files::remove_char_start(ClearClassNamePtr, 'F');

    }
    // 处理函数
    if (InClassAnalysis.Functions.size() > 0)
    {
        for (const FFunctionAnalysis& Function : InClassAnalysis.Functions)
        {
            if (Function.CodeType == "Describe")
            {
                std::string VMString = "VM_" + Function.FunctionName;
                //OutAnalysisRaw.push_back("BT_VM(" + VMString + ") \\");
                //OutAnalysisRaw.push_back("{ \\");
            }
            else if (Function.CodeType == "Event")
            {

            }
        }
        helper_tool_files::remove_char_end((char*)OutAnalysisRaw.back().c_str(), '\\');//移除最后的\，宏的最后一行不用\;
        OutAnalysisRaw.push_back("");

        // #define Z_BT_UParticleSystem_12_GENERATED_BODY_BT 
        OutAnalysisRaw.push_back(std::string("#define ") + InClassAnalysis.ClassName + "_" + to_string(InClassAnalysis.CodeLine) + "_GENERATED_BODY_BT" + "\\");
        //
        OutAnalysisRaw.push_back(MyClassName);
        OutAnalysisRaw.push_back("");

        OutAnalysisRaw.push_back("#define " + string("CURRENT_FILE_ID_BT ") + InClassAnalysis.ClassName);

        OutAnalysisRaw.push_back("#define" + string("NewLine ") + to_string(InClassAnalysis.CodeLine));
    }
}

void IntermediateFile::GenerateFile_CPP(const FClassAnalysis& InClassAnalysis, vector<string>& StaticRegistration, vector<string>& OutAnalysisRaw)
{
    OutAnalysisRaw.push_back("#ifdef _MSC_VER");
    OutAnalysisRaw.push_back("#pragma warning (push)");
    OutAnalysisRaw.push_back("#pragma warning (disable : 4883)");
    OutAnalysisRaw.push_back("#endif");
    OutAnalysisRaw.push_back("PRAGMA_DISABLE_DEPRECATION_WARNINGS");
    OutAnalysisRaw.push_back("");

    {

    }

    OutAnalysisRaw.push_back("");
    OutAnalysisRaw.push_back("PRAGMA_ENABLE_DEPRECATION_WARNINGS");
    OutAnalysisRaw.push_back("#ifdef _MSC_VER");
    OutAnalysisRaw.push_back("#pragma warning (pop)");
    OutAnalysisRaw.push_back("#endif");
}
