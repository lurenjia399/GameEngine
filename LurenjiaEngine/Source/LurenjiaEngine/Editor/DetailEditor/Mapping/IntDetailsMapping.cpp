#include "IntDetailsMapping.h"
#include "CoreObject/Property.h"

void FIntDetailsMapping::UpdateDetailsWidget(UProperty* InProperty)
{
	ImGui::InputInt(InProperty->GetName().c_str(), InProperty->GetData<int>());
}

std::shared_ptr<FIntDetailsMapping> FIntDetailsMapping::MakeDetilsMapping()
{
	return std::make_shared<FIntDetailsMapping>();
}
