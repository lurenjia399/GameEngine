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

	bool Collection(const string& Paths, FClassAnalysis& OutClassAnalysis);

	// ��ȡfunction��CodeType
	bool GetCodeTypeByFunc(const string& RowString, FFunctionAnalysis& FunctionAnalysis);
	// ��ȡ����������
	FParamElement CollectionVariableType(const string& RowString, ECollectionParamType Type);
}
