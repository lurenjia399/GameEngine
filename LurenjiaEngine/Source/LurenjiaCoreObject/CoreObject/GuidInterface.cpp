#include "GuidInterface.h"

IGuidInterface::IGuidInterface()
{
	create_guid(&Guid);

	// char* 必须在初始化列表中初始化，明天改掉
	memset(name, 0, sizeof(char)*50);
	memset(RealityGuid, 0, 50);
	
	//ResetGuid("");
}

void IGuidInterface::ResetGuid(const char* inName)
{
	//name = inName;
	
	int count = (int)strlen(inName);
	memcpy_s(name, (int)strlen(inName)+1, inName, (int)strlen(inName)+1);
	

	char tem[33] = { '\0' };
	char* newGuid = tem;
	guid_to_string(newGuid, &Guid);
	string str(newGuid);

	string inName_str = inName;
	string str_str = str;
	//RealityGuid = (inName_str + str_str).c_str();
	memcpy_s(RealityGuid, (int)strlen(newGuid) + 1, newGuid, (int)strlen(newGuid) + 1);
}
