#pragma

#include "../../simple_library/public/simple_library.h"
#include "../CodeType.h"
#include "../Tool/HelperTool.h"

namespace CollectClassInfo
{
	const char SpaceString[] = " ";
	const char ColonString[] = ":";
	const char CommaString[] = ",";
	const char LeftHalfRadius[] = "(";
	const char StarString[] = "*";
	const char ReferenceString[] = "&";
	const char CodeTypeString[] = "CodeType";
	const char EqualString[] = "=";

	bool Collection(const string& Paths, FClassAnalysis& OutClassAnalysis);

	// 获取function的CodeType
	bool GetCodeTypeByFunc(const string& RowString, FFunctionAnalysis& OutFunctionAnalysis);
	// 获取变量的CodeType
	bool GetCodeTypeByVariable(const string& RowString, FVariableAnalysis& OutVariableAnalysis);
	// 获取参数的类型
	FParamElement CollectionVariableType(const char* RowString, ECollectionParamType Type);
	// 去掉参数的默认值
	char* RemoveVariableDefaultValue(const char* RowString);
	
}
