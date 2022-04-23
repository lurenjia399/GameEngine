#include "TransformationComponent.h"
#include "../Mesh/Core/ObjectTransformation.h"

CTransformationComponent::CTransformationComponent()
	: Position(0.f, 0.f, 0.f)
	, ForwardVector(1.0f, 0.f, 0.f)
	, RightVector(0.f, 1.f, 0.0f)
	, UpVector(0.f, 0.0f, 1.f)
{
}

void CTransformationComponent::CorrectionVector(fvector_3d& InV3)
{
	//XMVECTOR Right = XMLoadFloat3(&RightVector);
	//XMVECTOR Up = XMLoadFloat3(&UpVector);
	//XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	//Forward = XMVector3Normalize(Forward);
	//Up = XMVector3Normalize(XMVector3Cross(Forward, Right));
	//Right = XMVector3Normalize(XMVector3Cross(Up, Forward));

	//XMStoreFloat3(&RightVector, Right);
	//XMStoreFloat3(&UpVector, Up);
	//XMStoreFloat3(&ForwardVector, Forward);
	InV3.x = Position.x;
	InV3.y = Position.y;
	InV3.z = Position.z;

}

void CTransformationComponent::GetCorrectionPosition(fvector_3d& InV3)
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);
	XMVECTOR NewPosition = XMLoadFloat3(&Position);

	InV3.x = XMVectorGetX(XMVector3Dot(Right, NewPosition));
	InV3.y = XMVectorGetX(XMVector3Dot(Up, NewPosition));
	InV3.z = XMVectorGetX(XMVector3Dot(Forward, NewPosition));
}
