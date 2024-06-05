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

	simple_c_guid Guid;			//���ɵ�guid
	const char* name;				//���������
	const char* RealityGuid;			//�ɶ������ƺ�guid����ϣ���ʵ�Ķ����ʶ����
};