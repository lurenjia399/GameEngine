#include "MapDetailsMapping.h"
#include "CoreObject/Property.h"

void FMapDetailsMapping::UpdateDetailsWidget(UProperty* InProperty)
{
}

std::shared_ptr<FMapDetailsMapping> FMapDetailsMapping::MakeDetilsMapping()
{
	return std::make_shared<FMapDetailsMapping>();
}
