#include "FloatDetailsMapping.h"
#include "CoreObject/Property.h"

void FFloatDetailsMapping::UpdateDetailsWidget(UProperty* InProperty)
{
	ImGui::InputFloat(InProperty->GetName().c_str(), InProperty->GetData<float>());
	//ImGui::DragFloat(InProperty->GetName().c_str(), InProperty->GetData<float>());
}

std::shared_ptr<FFloatDetailsMapping> FFloatDetailsMapping::MakeDetilsMapping()
{
	return std::make_shared<FFloatDetailsMapping>();
}
