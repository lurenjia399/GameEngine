#include "Vector3DetailsMapping.h"
#include "CoreObject/Property.h"

void FVector3DetailsMapping::UpdateDetailsWidget(UProperty* InProperty)
{
	ImGui::InputFloat3(InProperty->GetName().c_str(), (float*)InProperty->GetData<XMFLOAT3>());
}

std::shared_ptr<FVector3DetailsMapping> FVector3DetailsMapping::MakeDetilsMapping()
{
	return std::make_shared<FVector3DetailsMapping>();
}
