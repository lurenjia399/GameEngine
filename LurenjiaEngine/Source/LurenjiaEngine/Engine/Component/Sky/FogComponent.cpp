#include "FogComponent.h"

CFogComponent::CFogComponent()
	: FogColor(1.f, 1.f, 1.f, 1.f)
	, FogStart(5.0f)
	, FogRange(100.f)
	, FogHeight(9000.f)
	, FogTransparentCoefficient(0.f)
	, bDirty(false)
	, bValid(false)
{
}

void CFogComponent::SetFogColor(const fvector_color& FogColor)
{
	this->FogColor = FogColor;
	SetDirtyState(true);
}

void CFogComponent::SetFogStart(const float& FogStart)
{
	this->FogStart = FogStart;
	SetDirtyState(true);
}

void CFogComponent::SetFogRange(const float& FogRange)
{
	this->FogRange = FogRange;
	SetDirtyState(true);
}

void CFogComponent::SetDirtyState(const bool& DirtyState)
{
	this->bDirty = DirtyState;
}

void CFogComponent::SetFogHeight(const float& FogHeight)
{
	this->FogHeight = FogHeight;
	SetDirtyState(true);
}

void CFogComponent::SetFogTransparentCoefficient(const float& FogTransparentCoefficient)
{
	this->FogTransparentCoefficient = FogTransparentCoefficient;
	SetDirtyState(true);
}

void CFogComponent::SetFogValid(const bool& bValid)
{
	this->bValid = bValid;
}
