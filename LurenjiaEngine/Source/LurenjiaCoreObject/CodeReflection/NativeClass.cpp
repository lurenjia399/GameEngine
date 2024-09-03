#include "NativeClass.h"
#include "../Construction/ObjectConstruction.h"
#include "../CoreObject/Property.h"

FNativeClass::FNativeClass()
	: Outer(nullptr)
	, Property(nullptr)
	, FunctionMap({})
{
}

void FNativeClass::AddProperty(const std::string& PropertyName, const std::string& PropertyType, int PropertyCount, int PerPropertySize, void* InData)
{
	FCreateObjectParam Param(Outer, nullptr, PropertyName.c_str());
	UProperty* Ptr = LurenjiaEngine::CreateObject<UProperty>(Param);
	Ptr->SetCountValue(PropertyCount);
	Ptr->SetSizeValue(PerPropertySize);
	Ptr->SetDataValue(InData);
	Ptr->SetTypeValue(PropertyType);

	if (!Property)
	{
		Property = Ptr;
	}
	else
	{
		UProperty* StartPtr = Property;
		while (StartPtr != nullptr)
		{
			if (!StartPtr->Next)
			{
				StartPtr->Next = Ptr;
				StartPtr = nullptr;
			}
			else
			{
				StartPtr = dynamic_cast<UProperty*>(StartPtr->Next);
			}
		}
	}

}
