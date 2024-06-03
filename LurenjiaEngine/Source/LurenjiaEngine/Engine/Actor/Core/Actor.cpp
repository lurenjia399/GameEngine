#include "Actor.h"


AActor::AActor()
{
	RootComponent = LurenjiaEngine::CreateObject<CTransformationComponent>(this, "AActor::RootComponent");
}


void AActor::SetPosition(const XMFLOAT3& InPosition)
{
	RootComponent->SetPosition(InPosition);
}

void AActor::SetRotation(const fvector_3d& InRotation)
{
	RootComponent->SetRotation(InRotation);
}

void AActor::SetScale(const XMFLOAT3& InScale)
{
	RootComponent->SetScale(InScale);
}

XMFLOAT3 AActor::GetPosition() const
{
	return RootComponent->GetPosition();
}

fvector_3d AActor::GetRotation() const
{
	return RootComponent->GetRotation();
}

XMFLOAT3 AActor::GetScale() const
{
	return RootComponent->GetScale();
}

void AActor::SetForward(const XMFLOAT3& Forward) const
{
	RootComponent->SetForward(Forward);
}

void AActor::SetRight(const XMFLOAT3& Right) const
{
	RootComponent->SetRight(Right);
}

void AActor::SetUp(const XMFLOAT3& Up) const
{
	RootComponent->SetUp(Up);
}

XMFLOAT3 AActor::GetForward() const
{
	return RootComponent->GetForward();
}

XMFLOAT3 AActor::GetRight() const
{
	return RootComponent->GetRight();
}

XMFLOAT3 AActor::GetUp() const
{
	return RootComponent->GetUp();
}

void AActor::SetPickup(bool InPickup)
{
}

void AActor::SetVisible(bool InVisible)
{
	return RootComponent->SetVisible(InVisible);
}

bool AActor::GetVisible()
{
	return RootComponent->GetIsVisible();
}
