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
	const char PointString[] = "*";
	const char ReferenceString[] = "&";

	bool Collection(const string& Paths, FClassAnalysis& OutClassAnalysis);
}
