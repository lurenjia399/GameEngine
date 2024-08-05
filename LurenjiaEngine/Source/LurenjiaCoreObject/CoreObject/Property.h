#pragma
#include "Field.h"

class UProperty;
class LURENJIACOREOBJECT_API UProperty : public UField
{
	typedef UField UField;
public:
	UProperty();

	// 返回UProperty的大小
	int GetSize() const { return 0; }

	FORCEINLINE void InitializeValue(void* Dest) const {}

};