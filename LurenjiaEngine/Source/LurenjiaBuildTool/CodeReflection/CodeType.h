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

	FParamElement():Name("null"), bConst(false), bPointer(false), bReference(false), Type("null")
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

	FFunctionAnalysis(): ParamArray({}), ReturnParam({}), FunctionName(""), bStatic(false), bVirtual(false), CodeType("")
	{}

};

// 类
struct FClassAnalysis
{
	std::vector<FFunctionAnalysis> Functions;//函数
	std::vector<FVariableAnalysis> Variable;//变量

	string APIName;//api名称
	string ClassName;//类名称

	std::vector<string> InheritName;//继承者名称
	int CodeLine;//反射宏在哪一行？
};

// 收集的参数类型
enum class ECollectionParamType
{
	TYPE_PARAM,	// 作为参数
	TYPE_RETURN, // 作为返回值
};