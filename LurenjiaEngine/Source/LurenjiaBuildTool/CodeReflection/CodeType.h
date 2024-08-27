#pragma once
#include <string>
#include <vector>
using namespace std;


// 参数
struct FParamElement
{
	string Name;			//参数名称
	bool bConst;			//const参数？
	bool bPointer;			//指针参数？
	bool bReference;		//引用参数？
	string Type;			//参数类型
	vector<FParamElement> InternalType;//内部类型,vector<int>这种的
	string Category;		//参数分类

	FParamElement()
		: Name("null")
		, bConst(false)
		, bPointer(false)
		, bReference(false)
		, Type("null")
		, Category("Default")
	{}
};

// 变量
struct FVariableAnalysis : public FParamElement
{
	string CodeType; //代码类型，类似ue里面uproperty里面写的参数
	bool bStatic; // 静态变量？
	FVariableAnalysis() :CodeType(""), bStatic(false)
	{}
};

// 函数
struct FFunctionAnalysis
{
	std::vector<FParamElement> ParamArray;//函数的参数
	FParamElement ReturnParam;//函数的返回值

	string FunctionName;//函数名字
	bool bStatic;//静态函数？
	bool bVirtual;//虚函数？
	string CodeType;//代码类型，类似ue里面ufunction里面写的参数
	string Category;//函数分类
	FFunctionAnalysis()
		: ParamArray({})
		, ReturnParam({})
		, FunctionName("")
		, bStatic(false)
		, bVirtual(false)
		, CodeType("")
		, Category("Default")
	{}

};

// 类
struct FClassAnalysis
{
	std::vector<FFunctionAnalysis> Functions;//函数
	std::vector<FVariableAnalysis> Variable;//变量

	string APIName;//api名称
	string ClassName;//类名称

	std::vector<string> FatherName;//父类名称
	string CodeCPPName;//类名称去掉前缀的
	string Filename;//.h文件路径
	int CodeLine;//反射宏在哪一行？

	string ModulePath;//模块路径
};

// 收集的参数类型
enum class ECollectionParamType
{
	TYPE_PARAM,	// 作为参数
	TYPE_RETURN, // 作为返回值
};

bool bIsCheckAllowCodeReflection(const std::vector<std::string>& InContent);