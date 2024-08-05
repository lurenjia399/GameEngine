#pragma
#include "Struct.h"

class UProperty;
class LURENJIACOREOBJECT_API UFunction : public UStruct
{
	typedef UField UStruct;
public:
	UFunction();

	UProperty* FirstPropertyToInit;
	// ue好像把Script放到UStruct里面了
	vector<unsigned char> Script;// 如果蓝图中执行一个事件，就会执行很多逻辑，这个Script就是存储这些逻辑的吧

};