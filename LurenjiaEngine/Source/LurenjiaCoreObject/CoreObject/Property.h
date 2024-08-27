#pragma
#include "Field.h"

// 不抛出4251警告
#pragma warning(push)
#pragma warning(disable:4251)

class UProperty;
class LURENJIACOREOBJECT_API UProperty : public UField
{
	typedef UField Super;
public:
	UProperty();
	template<typename T>
	FORCEINLINE T* GetData() const
	{
		return (T*)Data;
	}
	FORCEINLINE void* GetData() const { return Data;
	}
	FORCEINLINE int GetCount() const { return Count; }
	FORCEINLINE int GetSize() const { return Size; }
	FORCEINLINE const string& GetType() const { return Type; }

	FORCEINLINE void SetDataValue(void* InDest) { Data = InDest; }
	FORCEINLINE void SetSizeValue(int InSize) { Size = InSize; }
	FORCEINLINE void SetCountValue(int InCount) { Count = InCount; }
	FORCEINLINE void SetTypeValue(const string& InType) { Type = InType; }

private:
	void* Data;//属性首地址
	int Count;//属性数量
	int Size;//属性大小
	string Type;//属性类型
};

#pragma warning(pop)