#include "BoolDetailsMapping.h"
#include "CoreObject/Property.h"

void FBoolDetailsMapping::UpdateDetailsWidget(UProperty* InProperty)
{
	ImGui::Checkbox(InProperty->GetName().c_str(), InProperty->GetData<bool>());
}

std::shared_ptr<FBoolDetailsMapping> FBoolDetailsMapping::MakeDetilsMapping()
{
	return std::make_shared<FBoolDetailsMapping>();
}
