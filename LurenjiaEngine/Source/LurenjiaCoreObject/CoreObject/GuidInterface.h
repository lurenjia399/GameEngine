#pragma once
#include "../CoreObjectMacro.h"
#include "../CoreObjectMinimal.h"

//template class LURENJIACOREOBJECT_API std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
// 不抛出4251警告
#pragma warning(push)
#pragma warning(disable:4251)
class LURENJIACOREOBJECT_API IGuidInterface
{
public:
	IGuidInterface();
	bool operator==(IGuidInterface* other)
	{
		return name == other->name && guid_equal(&this->Guid, &other->Guid);
	}

	const string GetGuid() const { return RealityGuid; }
	string GetName() { return name; }
	void ResetGuid(string inName);
private:

	simple_c_guid Guid;			//生成的guid
	
	string name;				//对象的名称
	string RealityGuid;			//由对象名称和guid的组合（真实的对象标识符）
};

#pragma warning(pop)