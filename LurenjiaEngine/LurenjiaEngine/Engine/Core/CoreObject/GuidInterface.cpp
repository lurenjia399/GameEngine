#include "GuidInterface.h"

IGuidInterface::IGuidInterface()
{
	create_guid(&Guid);
	ResetGuid("");
}

void IGuidInterface::ResetGuid(string inName)
{
	name = inName;
	char tem[33] = { '\0' };
	char* newGuid = tem;
	guid_to_string(newGuid, &Guid);
	string str(newGuid);
	RealityGuid = inName + str;
}
