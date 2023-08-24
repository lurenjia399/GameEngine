#pragma once
#include "../../LurenjiaEngine.h"

class IGuidInterface
{
public:
	IGuidInterface();
	bool operator==(IGuidInterface* other)
	{
		return name == other->name && guid_equal(&this->Guid, &other->Guid);
	}

	string GetGuid() { return RealityGuid; }
	string GetName() { return name; }
	void ResetGuid(string inName);
private:

	simple_c_guid Guid;			//生成的guid
	string name;				//对象的名称
	string RealityGuid;			//由对象名称和guid的组合（真实的对象标识符）
};