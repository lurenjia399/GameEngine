#include "NativeClass.h"

FNativeClass::FNativeClass()
	: Outer(nullptr)
	, FunctionMap({})
{
}

void FNativeClass::AddProperty(const std::string& PropertyName, const std::string& PropertyType, int PropertyCount, int PerPropertySize, void* InData)
{

}
