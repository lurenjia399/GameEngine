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

	// 注意这个旋转矩阵表示的是，在世界空间下，摄像机的旋转
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

	// 注意这里，通过摄像机获取的摄像机矩阵是主列的，我们在代码中都是主行的，也就是在使用的时候需要转置
	XMStoreFloat4x4(&ViewMatrix, rotateMatrix * TranslateMatrix);
}

void AClientViewport::BuildOrthMatrix(float InRadius, const XMFLOAT3& InTargetPoint)
{
	XMVECTOR TargetPosition = XMLoadFloat3(&InTargetPoint);
	XMMATRIX ViewMatrix_xm = XMLoadFloat4x4(&ViewMatrix);

	XMFLOAT3 ViewCenter;
	XMStoreFloat3(&ViewCenter, XMVector3TransformCoord(TargetPosition, ViewMatrix_xm));

	float ViewLeft = ViewCenter.y - InRadius;
	float ViewRight = ViewCenter.y + InRadius;
	float ViewBottom = ViewCenter.z - InRadius;
	float ViewTop = ViewCenter.z + InRadius;
	float NearZ = ViewCenter.x - InRadius;
	float FarZ = ViewCenter.x + InRadius;

	XMMATRIX orthoMatrix_xm = XMMatrixOrthographicOffCenterLH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
	XMStoreFloat4x4(&ProjectMatrix, orthoMatrix_xm);
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

	// 创建新的透视投影矩阵
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

	// 重新矫正up向量
	XMVECTOR up_new = XMVector3Cross(forward, right);
	up_new = XMVector3Normalize(up);
	
	SetForward(XMFLOAT3(forward.m128_f32[0], forward.m128_f32[1], forward.m128_f32[2]));
	SetRight(XMFLOAT3(right.m128_f32[0], right.m128_f32[1], right.m128_f32[2]));
	SetUp(XMFLOAT3(up_new.m128_f32[0], up_new.m128_f32[1], up_new.m128_f32[2]));

	SetDirty(true);

}
