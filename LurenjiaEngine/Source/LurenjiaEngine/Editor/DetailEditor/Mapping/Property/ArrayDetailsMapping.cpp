#include "ArrayDetailsMapping.h"
#include "CoreObject/Property.h"

void FArrayDetailsMapping::UpdateDetailsWidget(UProperty* InProperty)
{
	
}

std::shared_ptr<FArrayDetailsMapping> FArrayDetailsMapping::MakeDetilsMapping()
{
	return std::make_shared<FArrayDetailsMapping>();
}
