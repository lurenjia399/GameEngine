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

	simple_c_guid Guid;			//���ɵ�guid
	string name;				//���������
	string RealityGuid;			//�ɶ������ƺ�guid����ϣ���ʵ�Ķ����ʶ����
};