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
    OutAnalysisRaw.push_back("#include \"CoreObject/CoreMinimalObject.h\"");
    OutAnalysisRaw.push_back("#include \"CodeReflection/Frame.h\"");
    OutAnalysisRaw.push_back("#include \"CodeReflection/ScriptMacro.h\"");
    OutAnalysisRaw.push_back("");

    std::string MyClassName = "Z_LRJ_" + InClassAnalysis.ClassName;// Z_LRJ_UParticleSystem

    //#define Z_LRJ_UParticleSystem \ 
    string temp = InClassAnalysis.Functions.size() > 0 ? "\\" : "";
    OutAnalysisRaw.push_back(helper_tool_files::printf(
            "#define %s %s",MyClassName.c_str(),temp.c_str()));

    // 这部分没用
    std::string ClearClassName = InClassAnalysis.ClassName;
    {
        char* ClearClassNamePtr = (char*)ClearClassName.c_str();

        helper_tool_files::trim_start_and_end_inline(ClearClassNamePtr);
        helper_tool_files::remove_char_start(ClearClassNamePtr, 'A');
        helper_tool_files::remove_char_start(ClearClassNamePtr, 'C');
        helper_tool_files::remove_char_start(ClearClassNamePtr, 'I');
        helper_tool_files::remove_char_start(ClearClassNamePtr, 'F');

    }
    // 处理函数,处理那种反射函数，蓝图可以调用的
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
            else if (Function.CodeType == "Function" ||
                Function.CodeType == "PureFunction")
            {
                /*
                    UFUNCTION(CodeType = Function)
	                static void Hello1(int c, float b) {}
                */
                //Script_Hello1
                std::string VMString = "Script_" + Function.FunctionName;
                //FUNCTION_DEFINITION(Script_Hello1) \
                //{\ 
                OutAnalysisRaw.push_back("FUNCTION_DEFINITION(" + VMString + ") \\");
                OutAnalysisRaw.push_back("{ \\");
                {
                    std::string VariableAdd;//Z_c_Name,Z_b_Name
                    //先拼接函数的参数
                    for (const FParamElement& Variable : Function.ParamArray)
                    {
                        std::string StackString = " Stack.GetParmAddr(); \\";

                        std::string VariableName = "Z_" + Variable.Name + "_Name";

                        //  float Z_a = *(float*)Stack.GetParmAddr();
                        OutAnalysisRaw.push_back("\t" + Variable.Type + (Variable.bPointer ? "* " : " ") +
                            VariableName + " = " + (Variable.bPointer ? "(" : "*(") +
                            Variable.Type + "*)" + StackString);
                        VariableAdd += ("," + VariableName);
                    }
                    char* VariableAddPtr = const_cast<char*>(VariableAdd.c_str());
                    helper_tool_files::remove_char_start(VariableAddPtr, ',');

                    //处理函数
                    if (Function.bStatic)
                    {
                        if (Function.ReturnParam.Type == "void")
                        {
                            //  UParticleSystem::Hello1(Z_c_Name,Z_b_Name);
                            OutAnalysisRaw.push_back(
                                helper_tool_files::printf(
                                    "\t%s::%s(%s); \\",
                                    InClassAnalysis.ClassName.c_str(),
                                    Function.FunctionName.c_str(),
                                    VariableAdd.c_str()));
                        }
                        else
                        {
                            //  *(std::string*)RefData = UParticleSystem::Hello1(Z_c_Name,Z_b_Name);
                            OutAnalysisRaw.push_back(
                                helper_tool_files::printf(
                                    "\t%s(%s*) RefData = %s::%s(%s); \\",
                                    string(Function.ReturnParam.bPointer ? "" : "*").c_str(),
                                    Function.ReturnParam.Type.c_str(),
                                    InClassAnalysis.ClassName.c_str(),
                                    Function.FunctionName.c_str(),
                                    VariableAdd.c_str()));
                        }
                    }
                    else //处理成员函数
                    {

                    }
                }
                //}\ 
                OutAnalysisRaw.push_back("} \\");

                //  FFuntionManage::SetNativeFuncPtr(FFuntionID(("ParticleSystem"),("Hello1"),UParticleSystem::Script_Hello1));
                StaticRegistration.push_back(
                    helper_tool_files::printf(
                        "\tFFuntionManage::SetNativeFuncPtr(FFuntionID((\"%s\"),(\"%s\"),%s::%s));",
                        ClearClassName.c_str(),
                        Function.FunctionName.c_str(),
                        InClassAnalysis.ClassName.c_str(),
                        VMString.c_str()));
            }
        }
        helper_tool_files::remove_char_end((char*)OutAnalysisRaw.back().c_str(), '\\');//移除最后的\，宏的最后一行不用\;
        OutAnalysisRaw.push_back("");
    }

    std::string MyInternalFuncName = helper_tool_files::printf(
        "%s_%i_Internal_Fun",
        InClassAnalysis.ClassName.c_str(), InClassAnalysis.CodeLine);// UParticleSystem_14_Internal_Fun
    //#define UParticleSystem_14_Internal_Fun \ 
    //typedef UFXSystemAsset Super0; \ 
    //protected: \ 
    //virtual void InitReflectionContent(); \ 
    //private:
    //
    OutAnalysisRaw.push_back(helper_tool_files::printf(
        "#define %s_%i_Internal_Fun \\", 
        InClassAnalysis.ClassName.c_str(), InClassAnalysis.CodeLine));
    for (int i = 0; i < InClassAnalysis.FatherName.size(); i++)
    {
        OutAnalysisRaw.push_back(helper_tool_files::printf(
            "typedef %s Super%i; \\",
            InClassAnalysis.FatherName[i].c_str(), i));
    }
    OutAnalysisRaw.push_back(helper_tool_files::printf(
        "protected: \\"));
    OutAnalysisRaw.push_back(helper_tool_files::printf(
        "virtual void InitReflectionContent(); \\"));
    OutAnalysisRaw.push_back(helper_tool_files::printf(
        "private: \\"));
    OutAnalysisRaw.push_back(helper_tool_files::printf(
        ""));

    //#define Z_BT_UParticleSystem_12_GENERATED_BODY_BT \
    //Z_LRJ_UParticleSystem \
    //UParticleSystem_14_Internal_Fun
    //
    
    OutAnalysisRaw.push_back(helper_tool_files::printf(
            "#define %s_%i_GENERATED_BODY_LRJ \\",InClassAnalysis.ClassName.c_str(),InClassAnalysis.CodeLine));
    OutAnalysisRaw.push_back(helper_tool_files::printf(
        "%s \\", MyClassName.c_str()));
    OutAnalysisRaw.push_back(MyInternalFuncName);
    OutAnalysisRaw.push_back("");

    //#ifdef CURRENT_FILE_ID_LRJ
    //#undef CURRENT_FILE_ID_LRJ
    //#endif
    //
    //#ifdef NewLine
    //#undef NewLine
    //#endif
    //
    //#define CURRENT_FILE_ID_LRJ UParticleSystem
    //#define NewLine 14
    OutAnalysisRaw.push_back("#ifdef CURRENT_FILE_ID_LRJ");
    OutAnalysisRaw.push_back("#undef CURRENT_FILE_ID_LRJ");
    OutAnalysisRaw.push_back("#endif");
    OutAnalysisRaw.push_back("");
    OutAnalysisRaw.push_back("#ifdef NewLine");
    OutAnalysisRaw.push_back("#undef NewLine");
    OutAnalysisRaw.push_back("#endif");
    OutAnalysisRaw.push_back("");
    OutAnalysisRaw.push_back("#define " + string("CURRENT_FILE_ID_LRJ ") + InClassAnalysis.ClassName);
    OutAnalysisRaw.push_back("#define " + string("NewLine ") + to_string(InClassAnalysis.CodeLine));
}

void IntermediateFile::GenerateFile_CPP(const FClassAnalysis& InClassAnalysis, vector<string>& StaticRegistration, vector<string>& OutAnalysisRaw)
{
    OutAnalysisRaw.push_back("===========================================================================*/");
    
    // #include "CodeReflectionTest.h"
    OutAnalysisRaw.push_back(helper_tool_files::printf(
        "#include \"%s\"",
        InClassAnalysis.Filename.c_str()));

    //反射的CodeReflectionTest.generated.h
    OutAnalysisRaw.push_back(helper_tool_files::printf(
        "#include \"%s.generated.h\"",
        InClassAnalysis.CodeCPPName.c_str()));

    OutAnalysisRaw.push_back("#include \"CodeReflection/FunctionManage.h\"");

    OutAnalysisRaw.push_back("");
    OutAnalysisRaw.push_back("#ifdef _MSC_VER");
    OutAnalysisRaw.push_back("#pragma warning (push)");
    OutAnalysisRaw.push_back("#pragma warning (disable : 4883)");
    OutAnalysisRaw.push_back("#endif");
    OutAnalysisRaw.push_back("PRAGMA_DISABLE_DEPRECATION_WARNINGS");
    OutAnalysisRaw.push_back("");

    {
        if (InClassAnalysis.Functions.size() > 0)
        {
            for (const FFunctionAnalysis& Function : InClassAnalysis.Functions)
            {
                //UFUNCTION(CodeType = Event)
                //static void Hello(int c, float b);

                if (Function.CodeType == "Event")
                {
                    std::string FunctionName = ("Name_") +
                        Function.FunctionName;

                    std::vector<std::string> ParamStr;
                    auto GetParmString = [&Function](std::vector<std::string>& ParamArray)->std::string
                        {
                            if (Function.ParamArray.size() == 0)
                            {
                                return "";
                            }
                            else
                            {
                                std::string ParamString;
                                for (auto& Param : Function.ParamArray)
                                {
                                    //int32 A 
                                    std::string NewParam = (Param.bConst ? ("const") : ("")) +
                                        Param.Type +
                                        (Param.bPointer ? ("* ") : (Param.bReference ? ("& ") : (" "))) +
                                        Param.Name;

                                    //int32 A,int32 B,int32 c
                                    ParamString += (",") + NewParam;

                                    ParamArray.push_back(NewParam);
                                }
                                helper_tool_files::remove_char_end(const_cast<char*>(ParamString.c_str()), ',');

                                return ParamString;
                            }
                        };

                    //static FName Name_Hello = FName(("Hello")); 
                    OutAnalysisRaw.push_back(
                        ("static std::string ") +
                        FunctionName +
                        (" = std::string((\"") +
                        Function.FunctionName +
                        ("\")); "));

                    // void UParticleSystem::Hello(int c, float b)
                    // {
                    OutAnalysisRaw.push_back(Function.ReturnParam.Type +
                        (Function.ReturnParam.bPointer ? ("*") : (Function.ReturnParam.bReference ? ("&") : (" "))) +
                        InClassAnalysis.ClassName + ("::") + Function.FunctionName +
                        ("(") + GetParmString(ParamStr) + (")"));
                    OutAnalysisRaw.push_back(("{"));

                    std::string StructName = ("Parm_") + Function.FunctionName;
                    if (Function.ParamArray.size() > 0)
                    {
                        //  struct FParm_Hello
                        //  {
                        //      int c;
                        //      float b;
                        //  };
                        OutAnalysisRaw.push_back(std::string(("\t")) + ("struct F") + StructName);
                        {
                            OutAnalysisRaw.push_back(std::string(("\t")) + ("{"));//{
                            for (auto& Param : ParamStr)
                            {
                                OutAnalysisRaw.push_back(std::string(("\t")) + std::string(("\t")) + Param + (";"));
                            }
                            OutAnalysisRaw.push_back(std::string(("\t")) + ("};"));//};
                        }
                        //  FParm_Hello Parm_Hello;
                        //  Parm_Hello.c = c;
                        //  Parm_Hello.b = b;
                        OutAnalysisRaw.push_back(std::string(("\t")) + ("F") + StructName + (" ") + StructName + (";"));
                        for (auto& Param : Function.ParamArray)
                        {
                            OutAnalysisRaw.push_back(std::string(("\t")) + StructName + (".") + Param.Name + (" = ") + Param.Name + (";"));
                        }
                    }
                    //  ExecutionScript(FindScriptFuntion(Hello),&Parm_Hello);
                    OutAnalysisRaw.push_back(std::string(("\t")) + ("ExecutionScript(FindScriptFuntion(") +
                        FunctionName + ("),") + (Function.ParamArray.size() == 0 ? ("NULL") : (("&") + StructName)) + (");"));
                    //}
                    OutAnalysisRaw.push_back(("} "));
                }
            }
        }
        OutAnalysisRaw.push_back((""));

        std::string Register_Func =
            helper_tool_files::printf("Register_%s()",
                InClassAnalysis.ClassName.c_str());
        //int Register_UParticleSystem()
        //{
        //  FFuntionManage::SetNativeFuncPtr(FFuntionID(("ParticleSystem"),("Hello1"),UParticleSystem::Script_Hello1));
        //  return 0;
        //}
        OutAnalysisRaw.push_back(
            helper_tool_files::printf("int %s", Register_Func.c_str()));
        OutAnalysisRaw.push_back(("{"));
        {
            //把static的插入进来
            OutAnalysisRaw.insert(
                OutAnalysisRaw.end(),
                StaticRegistration.begin(),
                StaticRegistration.end());

            OutAnalysisRaw.push_back((""));
            OutAnalysisRaw.push_back(("\treturn 0;"));
        }
        OutAnalysisRaw.push_back(("}"));

        //static int UParticleSystem_Index = Register_UParticleSystem();
        OutAnalysisRaw.push_back(
            helper_tool_files::printf(
                "static int %s_Index = %s;",
                InClassAnalysis.ClassName.c_str(),
                Register_Func.c_str()));
    }

    OutAnalysisRaw.push_back("");
    OutAnalysisRaw.push_back("PRAGMA_ENABLE_DEPRECATION_WARNINGS");
    OutAnalysisRaw.push_back("#ifdef _MSC_VER");
    OutAnalysisRaw.push_back("#pragma warning (pop)");
    OutAnalysisRaw.push_back("#endif");
}
