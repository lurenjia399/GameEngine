#include "TransformationComponent.h"
#include "../Mesh/Core/ObjectTransformation.h"

CTransformationComponent::CTransformationComponent()
	: Position(-4.f, 0.f, 0.f)
	, Rotation(0.0f, 0.0f, 0.0f)
	, Scale(1.f, 1.f, 1.f)
	, ForwardVector(1.f, 0.f, 0.f)
	, RightVector(0.f, 1.f, 0.0f)
	, UpVector(0.f, 0.0f, 1.f)
{
}

void CTransformationComponent::SetPosition(const XMFLOAT3& InPosition)
{
	Position = InPosition;
}

void CTransformationComponent::SetRotation(const fvector_3d& InRotation)
{
	Rotation = XMFLOAT3(InRotation.x, InRotation.y, InRotation.z);

	float pitchRadians = XMConvertToRadians(InRotation.x);
	float yawRadians = XMConvertToRadians(InRotation.y);
	float rollRadians = XMConvertToRadians(InRotation.z);

	XMMATRIX RotateMatrix = XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians);

	XMVECTOR right = XMLoadFloat3(&RightVector);
	XMVECTOR up = XMLoadFloat3(&UpVector);
	XMVECTOR forward = XMLoadFloat3(&ForwardVector);

	XMStoreFloat3(&RightVector, XMVector2TransformNormal(right, RotateMatrix));
	XMStoreFloat3(&UpVector, XMVector2TransformNormal(up, RotateMatrix));
	XMStoreFloat3(&ForwardVector, XMVector2TransformNormal(forward, RotateMatrix));

	NormalizeTransformationVector();
}

void CTransformationComponent::SetScale(const XMFLOAT3& InScale)
{
	Scale = InScale;
	//Scale.y = InScale.z;
	//Scale.z = InScale.x;
	//Scale.x = InScale.y;
}

void CTransformationComponent::SetForward(const XMFLOAT3& InForward)
{
	ForwardVector = InForward;
}

void CTransformationComponent::SetRight(const XMFLOAT3& InRight)
{
	RightVector = InRight;
}

void CTransformationComponent::SetUp(const XMFLOAT3& InUp)
{
	UpVector = InUp;
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
