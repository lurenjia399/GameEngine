#include "Camera.h"
#include "../Component/TransformationComponent.h"
#include "../Component/InputComponent.h"

CCamera::CCamera()
{
	InputComponent = CreateObject<CInputComponent>("InputComponent");
	TransformationComponent = CreateObject<CTransformationComponent>("TransformationComponent");
}

void CCamera::BeginInit()
{
	ViewportInit();	//初始化vp矩阵

	InputComponent->CaptureKeyboardInforDelegate.Bind(this, &CCamera::ExecuteKeyboard);
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &CCamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &CCamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &CCamera::OnMouseMove);
}

void CCamera::Tick(float DeltaTime)
{
	BulidViewMatrix(DeltaTime);
}

void CCamera::BulidViewMatrix(float DeltaTime)
{
	fvector_3d V3;
	TransformationComponent->CorrectionVector(V3);

	TransformationComponent->GetCorrectionPosition(V3);

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
	XMMATRIX PosMatrix =
	{
		1.f, 0.f, 0.f, -PositionVector.x,
		0.f, 1.f, 0.f, -PositionVector.y,
		0.f, 0.f, 1.f, -PositionVector.z,
		0.f, 0.f, 0.f, 1.f
	};
	XMStoreFloat4x4(&ViewMatrix, rotateMatrix * PosMatrix);//XMMatrixTranspose(rotateMatrix * PosMatrix));

}

void CCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
	if (InputKey.KeyName == "W")
	{
		MoveForward(1.f);
	}
	else if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);
	}
	else if (InputKey.KeyName == "D")
	{
		MoveRight(1.0f);
	}
	else if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);
	}
}

void CCamera::OnMouseButtonDown(int X, int Y)
{
	LastMousePosition.x = X;
	LastMousePosition.y = Y;
}

void CCamera::OnMouseButtonUp(int X, int Y)
{
}

void CCamera::OnMouseMove(int X, int Y)
{

}

void CCamera::MoveForward(float InValue)
{
	XMFLOAT3 newPos = TransformationComponent->GetPosition();
	XMFLOAT3 forward = TransformationComponent->GetForward();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
	XMVECTOR Forward = XMLoadFloat3(&forward);
	XMVECTOR Position = XMLoadFloat3(&newPos);

	XMStoreFloat3(&newPos, XMVectorMultiplyAdd(AmountMovement, Forward, Position));//从Positon点沿着Forward方向移动AmountMovement距离
	TransformationComponent->SetPosition(newPos);
}

void CCamera::MoveRight(float InValue)
{
}
