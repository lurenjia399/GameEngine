#include "Actor.h"


AActor::AActor()
{
	TransformationComponent = LurenjiaEngine::CreateObject<CTransformationComponent>("AActor::TransformationComponent");
}

void AActor::SetPosition(const XMFLOAT3& InPosition)
{
	TransformationComponent->SetPosition(InPosition);
}

void AActor::SetRotation(const fvector_3d& InRotation)
{
	TransformationComponent->SetRotation(InRotation);
}

void AActor::SetScale(const XMFLOAT3& InScale)
{
	TransformationComponent->SetScale(InScale);
}

XMFLOAT3 AActor::GetPosition() const
{
	return TransformationComponent->GetPosition();
}

fvector_3d AActor::GetRotation() const
{
	return TransformationComponent->GetRotation();
}

XMFLOAT3 AActor::GetScale() const
{
	return TransformationComponent->GetScale();
}

XMFLOAT3 AActor::GetForward() const
{
	return TransformationComponent->GetForward();
}

XMFLOAT3 AActor::GetRight() const
{
	return TransformationComponent->GetRight();
}

XMFLOAT3 AActor::GetUp() const
{
	return TransformationComponent->GetUp();
}