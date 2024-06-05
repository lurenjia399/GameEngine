#pragma once
#include "../CoreObjectMacro.h"
#include "../CoreObjectMinimal.h"

class LURENJIACOREOBJECT_API IGuidInterface
{
public:
	IGuidInterface();
	bool operator==(IGuidInterface* other)
	{
		return name == other->name && guid_equal(&this->Guid, &other->Guid);
	}

	const char* GetGuid() const { return RealityGuid; }
	const char* GetName() const { return name; }
	void ResetGuid(const char* inName);
private:

	simple_c_guid Guid;			//生成的guid
	const char* name;				//对象的名称
	const char* RealityGuid;			//由对象名称和guid的组合（真实的对象标识符）
};