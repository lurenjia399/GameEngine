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

	string GetGuid() const { return RealityGuid; }
	string GetName() const { return name; }
	void ResetGuid(const string& inName);
private:

	simple_c_guid Guid;			//���ɵ�guid
	string name;				//���������
	string RealityGuid;			//�ɶ������ƺ�guid����ϣ���ʵ�Ķ����ʶ����
};