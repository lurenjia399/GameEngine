#include "TransformationComponent.h"
#include "../Actor/Mesh/Core/ObjectConstantBuffer.h"

CTransformationComponent::CTransformationComponent()
	: Position(0.f, 0.f, 0.f)
	, Rotation(0.0f, 0.0f, 0.0f)
	, Scale(1.f, 1.f, 1.f)
	, ForwardVector(1.f, 0.f, 0.f)
	, RightVector(0.f, 1.f, 0.0f)
	, UpVector(0.f, 0.0f, 1.f)
	, PositionOffset(0.f, 0.f, 0.f)
	, AttachParent({})
	, AttachChildren({})
	, bIsVisible(true)
{
}

void CTransformationComponent::SetPosition(const XMFLOAT3& InPosition)
{
	UpdateComponentPositionToWorldWithParent(GetAttachParent(), InPosition);
}

void CTransformationComponent::SetRotation(const fvector_3d& InRotation)
{
	UpdateComponentRotationToWorldWithParent(GetAttachParent(), InRotation);
}

void CTransformationComponent::SetScale(const XMFLOAT3& InScale)
{
	UpdateComponentScaleToWorldWithParent(GetAttachParent(), InScale);
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

void CTransformationComponent::SetPositionOffset(const XMFLOAT3& InPosition)
{
	PositionOffset = InPosition;
}

XMFLOAT3& CTransformationComponent::GetPosition()
{
	//if (!AttachChildren.empty())
	//{
	//	for (std::weak_ptr<CTransformationComponent> children : AttachChildren)
	//	{
	//		XMFLOAT3 ChildrenPosition = children.lock()->GetPosition();
	//		Engine_Log_Error("CTransformationComponent::ChildrenPosition position x[%f], y[%f], z[%f]", ChildrenPosition.x, ChildrenPosition.y, ChildrenPosition.z)
	//	}
	//}

	return Position;
}

void CTransformationComponent::NormalizeTransformationVector()
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	Forward = XMVector3Normalize(Forward);
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));

	Right = XMVector3Cross(Up, Forward);
	//Engine_Log("Forward [x] = %f, [y] = %f, [z] = %f", Forward.m128_f32[0], Forward.m128_f32[1], Forward.m128_f32[2]);
	XMStoreFloat3(&RightVector, Right);
	XMStoreFloat3(&UpVector, Up);
	XMStoreFloat3(&ForwardVector, Forward);

}

std::weak_ptr<CTransformationComponent> CTransformationComponent::GetAttachParent()
{
	return AttachParent;
}

void CTransformationComponent::AttachToComponent(std::weak_ptr<CTransformationComponent> Parent)
{
	SetAttachParent(Parent);

	Parent.lock()->AttachChildren.push_back(std::static_pointer_cast<CTransformationComponent>(shared_from_this()));
}

void CTransformationComponent::SetAttachParent(std::weak_ptr<CTransformationComponent> Parent)
{
	AttachParent = Parent;
}

void CTransformationComponent::UpdateComponentPositionToWorldWithParent(std::weak_ptr<CTransformationComponent> Parent, const XMFLOAT3& InPosition)
{

	//if (Parent.lock())
	//{
	//	Parent.lock()->UpdateComponentToWorldWithParent(GetAttachParent(), InPosition);
	//}

	Position = InPosition;

	if (!AttachChildren.empty())
	{
		for (std::weak_ptr<CTransformationComponent> children: AttachChildren)
		{
			children.lock()->UpdateChildPosition(InPosition);
		}
	}
}

void CTransformationComponent::UpdateComponentRotationToWorldWithParent(std::weak_ptr<CTransformationComponent> Parent, const fvector_3d& InRotation)
{
	Rotation = XMFLOAT3(InRotation.x, InRotation.y, InRotation.z);

	float pitchRadians = XMConvertToRadians(InRotation.x);
	float yawRadians = XMConvertToRadians(InRotation.y);
	float rollRadians = XMConvertToRadians(InRotation.z);
	XMMATRIX RotateMatrix = XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians);
	XMVECTOR right = XMLoadFloat3(&RightVector);
	XMVECTOR up = XMLoadFloat3(&UpVector);
	XMVECTOR forward = XMLoadFloat3(&ForwardVector);
	XMStoreFloat3(&RightVector, XMVector3TransformNormal(right, RotateMatrix));
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(up, RotateMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(forward, RotateMatrix));
	NormalizeTransformationVector();


	// КЂзга§зЊ
	if (!AttachChildren.empty())
	{
		for (std::weak_ptr<CTransformationComponent> children : AttachChildren)
		{
			children.lock()->UpdateChildRotation(InRotation);
		}
	}
}

void CTransformationComponent::UpdateComponentScaleToWorldWithParent(std::weak_ptr<CTransformationComponent> Parent, const XMFLOAT3& InScale)
{
	Scale = InScale;

	if (!AttachChildren.empty())
	{
		for (std::weak_ptr<CTransformationComponent> children : AttachChildren)
		{
			children.lock()->UpdateChildScale(InScale);
		}
	}
}

void CTransformationComponent::UpdateChildPosition(const XMFLOAT3& InPosition)
{
	XMFLOAT3 newLoation = XMFLOAT3(PositionOffset.x + InPosition.x, PositionOffset.y + InPosition.y, PositionOffset.z + InPosition.z);
	Position = newLoation;
}

void CTransformationComponent::UpdateChildRotation(const fvector_3d& InRotation)
{
	fvector_3d newRotation = fvector_3d(Rotation.x + InRotation.x, Rotation.y + InRotation.y, Rotation.z + InRotation.z);
	Rotation = XMFLOAT3(newRotation.x, newRotation.y, newRotation.z);
	float pitchRadians = XMConvertToRadians(InRotation.x);
	float yawRadians = XMConvertToRadians(InRotation.y);
	float rollRadians = XMConvertToRadians(InRotation.z);
	XMMATRIX RotateMatrix = XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians);
	XMVECTOR right = XMLoadFloat3(&RightVector);
	XMVECTOR up = XMLoadFloat3(&UpVector);
	XMVECTOR forward = XMLoadFloat3(&ForwardVector);
	XMStoreFloat3(&RightVector, XMVector3TransformNormal(right, RotateMatrix));
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(up, RotateMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(forward, RotateMatrix));
	NormalizeTransformationVector();
}

void CTransformationComponent::UpdateChildScale(const XMFLOAT3& InScale)
{
	XMFLOAT3 newScale = XMFLOAT3(InScale.x, InScale.y, InScale.z);
	Scale = newScale;
}

void CTransformationComponent::SetVisible(bool bNewVisible)
{
	bIsVisible = bNewVisible;

	if (!AttachChildren.empty())
	{
		for (std::weak_ptr<CTransformationComponent> children : AttachChildren)
		{
			children.lock()->SetVisible(bNewVisible);
		}
	}
}

bool CTransformationComponent::GetIsVisible() const
{
	return bIsVisible;
}
