#include "TransformationComponent.h"
#include "../Mesh/Core/ObjectTransformation.h"

CTransformationComponent::CTransformationComponent()
	: Position(-4.f, 0.f, 0.f)
	, ForwardVector(1.f, 0.f, 0.f)
	, RightVector(0.f, 1.f, 0.0f)
	, UpVector(0.f, 0.0f, 1.f)
{
}

void CTransformationComponent::NormalizeTransformationVector()
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	Forward = XMVector3Normalize(Forward);
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));

	Right = XMVector3Cross(Up, Forward);

	XMStoreFloat3(&RightVector, Right);
	XMStoreFloat3(&UpVector, Up);
	XMStoreFloat3(&ForwardVector, Forward);

}

void CTransformationComponent::UpdateCameraInfo()
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);
}
