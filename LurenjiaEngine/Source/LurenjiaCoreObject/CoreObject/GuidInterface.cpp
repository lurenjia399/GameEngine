#include "GuidInterface.h"

IGuidInterface::IGuidInterface()
{
	create_guid(&Guid);
	ResetGuid("");
}

void IGuidInterface::ResetGuid(const char* inName)
{
	name = inName;
	char tem[33] = { '\0' };
	char* newGuid = tem;
	guid_to_string(newGuid, &Guid);
	string str(newGuid);

	string inName_str = inName;
	string str_str = str;
	RealityGuid = (inName_str + str_str).c_str();
}
