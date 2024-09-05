#include "ActorDetailsMapping.h"
#include "CoreObject/Property.h"

void FActorDetailsMapping::UpdateDetailsWidget(CCoreMinimalObject* InCoreMinimalObject)
{
}

std::shared_ptr<FActorDetailsMapping> FActorDetailsMapping::MakeDetilsMapping()
{
	return std::make_shared<FActorDetailsMapping>();
}
