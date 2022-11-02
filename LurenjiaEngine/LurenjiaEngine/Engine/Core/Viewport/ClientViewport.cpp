#include "ClientViewport.h"

AClientViewport::AClientViewport()
	: SuperA::AActor()
	, SuperV::FViewport()
	, YFOV(0.f)
	, Aspect(0.f)
	, ZNear(0.f)
	, ZFar(0.f)
	, bDirty(false)
{
}


void AClientViewport::BulidViewMatrix(float DeltaTime)
{
	TransformationComponent->NormalizeTransformationVector();
	XMFLOAT3 RightVector = TransformationComponent->GetRight();
	XMFLOAT3 UpVector = TransformationComponent->GetUp();
	XMFLOAT3 ForwardVector = TransformationComponent->GetForward();
	XMFLOAT3 PositionVector = TransformationComponent->GetPosition();

	XMMATRIX rotateMatrix =
	{
		RightVector.x,	UpVector.x, ForwardVector.x,	0.f,
		RightVector.y,	UpVector.y, ForwardVector.y,	0.f,
		RightVector.z,	UpVector.z, ForwardVector.z,	0.f,
		0,				0,			0,					1.f
	};
	rotateMatrix = XMMatrixTranspose(rotateMatrix);
	XMMATRIX TranslateMatrix =
	{
		1.f, 0.f, 0.f, -PositionVector.x,
		0.f, 1.f, 0.f, -PositionVector.y,
		0.f, 0.f, 1.f, -PositionVector.z,
		0.f, 0.f, 0.f, 1.f
	};
	XMStoreFloat4x4(&ViewMatrix, rotateMatrix * TranslateMatrix);
}

void AClientViewport::Tick(float DeltaTime)
{
	BulidViewMatrix(DeltaTime);
}

void AClientViewport::SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar)
{
	YFOV = InYFOV;
	Aspect = InAspect;
	ZNear = InZNear;
	ZFar = InZFar;

	// �����µ�͸�Ӿ���
	XMMATRIX Project = XMMatrixPerspectiveFovLH(InYFOV, InAspect, InZNear, InZFar);
	XMStoreFloat4x4(&ProjectMatrix, Project);

	SetDirty(true);
}

void AClientViewport::FaceTarget(const XMFLOAT3& InPosition, const XMFLOAT3& InTargetPosition, const XMFLOAT3& InUpDirection)
{
	XMVECTOR forward = XMVectorSubtract(XMLoadFloat3(&InTargetPosition), XMLoadFloat3(&InPosition));
	forward = XMVector3Normalize(forward);

	XMVECTOR up = XMLoadFloat3(&InUpDirection);
	up = XMVector3Normalize(up);

	XMVECTOR right = XMVector3Cross(up, forward);
	right = XMVector3Normalize(right);

	// ���½���up����
	XMVECTOR up_new = XMVector3Cross(forward, right);
	up_new = XMVector3Normalize(up);
	
	SetForward(XMFLOAT3(forward.m128_f32[0], forward.m128_f32[1], forward.m128_f32[2]));
	SetRight(XMFLOAT3(right.m128_f32[0], right.m128_f32[1], right.m128_f32[2]));
	SetUp(XMFLOAT3(up_new.m128_f32[0], up_new.m128_f32[1], up_new.m128_f32[2]));

	SetDirty(true);

}
