#pragma once
#include "../CoreObjectMacro.h"

class CCoreMinimalObject;
class CComponent;
namespace ConstructionComponent
{
	void LURENJIACOREOBJECT_API UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent);
	void LURENJIACOREOBJECT_API ConstructionComponents(CCoreMinimalObject* InOuter, CCoreMinimalObject* NewObject);
}