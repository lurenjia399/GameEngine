#pragma once

#include "../CoreObject/CoreMinimalObject.h"

class LURENJIACOREOBJECT_API CComponent : public CCoreMinimalObject
{
public:
	CComponent();

	void SetParentComponent(CComponent* InParent) { Parent = InParent; }
	void AddChildren(CComponent* InChildren);

public:
	FORCEINLINE std::vector<CComponent*>& GetChildrens() { return Children; }

protected:
	CComponent* Parent;
	std::vector<CComponent*> Children;
};