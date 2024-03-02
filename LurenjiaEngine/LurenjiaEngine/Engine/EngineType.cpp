#include "EngineType.h"

FHitResult::FHitResult()
	: bHit(false)
	, Location(XMFLOAT3(0.f, 0.f, 0.f))
	, ImpactPoint(XMFLOAT3(0.f, 0.f, 0.f))
	, Normal(XMFLOAT3(0.f, 0.f, 0.f))
	, Distance(0.f)
	, Time(0.f)
	, Component_({})
	, Actor_({})
{

}
