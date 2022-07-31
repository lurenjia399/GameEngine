#include "Fog.h"
#include "../../Component/Sky/FogComponent.h"

AFog::AFog()
	: FogComponent(nullptr)
{
	FogComponent = LurenjiaEngine::CreateObject<CFogComponent>("FogComponent");
}

void AFog::SetFogColor(const fvector_color& FogColor)
{
	FogComponent->SetFogColor(FogColor);
}

void AFog::SetFogStart(const float& FogStart)
{
	FogComponent->SetFogStart(FogStart);
}

void AFog::SetFogRange(const float& FogRange)
{
	FogComponent->SetFogRange(FogRange);
}

void AFog::SetFogHeight(const float& FogHeight)
{
	FogComponent->SetFogHeight(FogHeight);
}

void AFog::SetFogTransparentCoefficient(const float& FogTransparentCoefficient)
{
	FogComponent->SetFogTransparentCoefficient(FogTransparentCoefficient);
}

void AFog::SetDirtyState(const bool& DirtyState)
{
	FogComponent->SetDirtyState(DirtyState);
}

CFogComponent* AFog::GetComponent() const
{
	return FogComponent;
}
