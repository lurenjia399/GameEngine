
#include "RegisterDetailsMapping.h"

std::map<std::string, shared_ptr<FClassDetailsMapping>> FRegisterDetailsMapping::ClassDetailsMap = {};
std::map<std::string, shared_ptr<FPropertyDetailsMapping>> FRegisterDetailsMapping::PropertyDetailsMap = {};

void FRegisterDetailsMapping::RegisterClassDetails(const std::string& InClassName, shared_ptr<FClassDetailsMapping> InMapping)
{
	if (InMapping && !InClassName.empty())
	{
		ClassDetailsMap.insert(std::pair<string, shared_ptr<FClassDetailsMapping>>(InClassName, InMapping));
	}
}

void FRegisterDetailsMapping::RegisterPropertyDetails(const std::string& InPropertyName, shared_ptr<FPropertyDetailsMapping> InMapping)
{
	if (InMapping && !InPropertyName.empty())
	{
		PropertyDetailsMap.insert(std::pair<string, shared_ptr<FPropertyDetailsMapping>>(InPropertyName, InMapping));
	}
}
