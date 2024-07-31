#include "GuidInterface.h"

IGuidInterface::IGuidInterface()
{
	create_guid(&Guid);

	//memset(name, 0, sizeof(char)*50);
	//memset(RealityGuid, 0, 50);
	
	ResetGuid("");
}

void IGuidInterface::ResetGuid(string inName)
{
	name = inName;

	char tem[33] = { '\0' };
	char* newGuid = tem;
	guid_to_string(newGuid, &Guid);
	string str(newGuid);

	string inName_str = inName;
	string str_str = str;
	RealityGuid = inName_str + str_str;
}
