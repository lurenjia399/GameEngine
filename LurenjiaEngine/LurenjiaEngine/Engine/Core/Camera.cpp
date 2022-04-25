#include "Camera.h"
#include "../Component/TransformationComponent.h"
#include "../Component/InputComponent.h"

CCamera::CCamera()
{
	InputComponent = CreateObject<CInputComponent>("InputComponent");
	TransformationComponent = CreateObject<CTransformationComponent>("TransformationComponent");

	Radius = 30.0f;
	verticalAngle = XM_PI;
	horizontalAngle = XM_PI;
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
	if (bLeftMouseDown)
	{
		TransformationComponent->NormalizeTransformationVector();
		XMFLOAT3 RightVector = TransformationComponent->GetRight();
		XMFLOAT3 UpVector = TransformationComponent->GetUp();
		XMFLOAT3& ForwardVector = TransformationComponent->GetForward();
		XMFLOAT3& PositionVector = TransformationComponent->GetPosition();

		PositionVector.x = Radius * sinf(verticalAngle) * cosf(horizontalAngle);
		PositionVector.y = Radius * cosf(verticalAngle);
		PositionVector.z = Radius * sinf(verticalAngle) * sinf(horizontalAngle);

		//ForwardVector.x = -PositionVector.x;
		//ForwardVector.y = -PositionVector.y;
		//ForwardVector.z = -PositionVector.z;

		TransformationComponent->NormalizeTransformationVector();

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
		XMStoreFloat4x4(&ViewMatrix, rotateMatrix * PosMatrix);
		//XMVECTOR Pos = XMVectorSet(PositionVector.x, PositionVector.y, PositionVector.z, 1.0f);
		//XMVECTOR ViewTarget = XMVectorZero();
		//XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

		//XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
		//XMStoreFloat4x4(&ViewMatrix, XMMatrixTranspose(ViewLookAt));
		return;
	}
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
	XMMATRIX PosMatrix =
	{
		1.f, 0.f, 0.f, -PositionVector.x,
		0.f, 1.f, 0.f, -PositionVector.y,
		0.f, 0.f, 1.f, -PositionVector.z,
		0.f, 0.f, 0.f, 1.f
	};
	XMStoreFloat4x4(&ViewMatrix, rotateMatrix * PosMatrix);

}

void CCamera::FocusMeshUpdateViewMatrix()
{
	TransformationComponent->NormalizeTransformationVector();
	XMFLOAT3 RightVector = TransformationComponent->GetRight();
	XMFLOAT3 UpVector = TransformationComponent->GetUp();
	XMFLOAT3& ForwardVector = TransformationComponent->GetForward();
	XMFLOAT3& PositionVector = TransformationComponent->GetPosition();

	PositionVector.x = Radius * sinf(verticalAngle) * cosf(horizontalAngle);
	PositionVector.y = Radius * cosf(verticalAngle);
	PositionVector.z = Radius * sinf(verticalAngle) * sinf(horizontalAngle);

	ForwardVector.x = -PositionVector.x;
	ForwardVector.y = -PositionVector.y;
	ForwardVector.z = -PositionVector.z;

	TransformationComponent->NormalizeTransformationVector();

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
	XMStoreFloat4x4(&ViewMatrix, rotateMatrix * PosMatrix);
}

void CCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
	if (InputKey.KeyName == "W")
	{
		MoveForward(1.f);
	}
	if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);
	}
	if (InputKey.KeyName == "D")
	{
		MoveRight(1.0f);
	}
	if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);
	}
	if (InputKey.KeyName == "E")
	{
		MoveUp(1.0f);
	}
	if (InputKey.KeyName == "Q")
	{
		MoveUp(-1.0f);
	}
	if (InputKey.KeyName == "F")
	{
		FocusMeshUpdateViewMatrix();
	}
}

void CCamera::OnMouseButtonDown(int X, int Y, string buttonType)
{
	if (buttonType == "R")
	{
		bRightMouseDown = true;
	}
	else if (buttonType == "L")
	{
		bLeftMouseDown = true;
	}
	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetMainWindowsHandle());
}

void CCamera::OnMouseButtonUp(int X, int Y, string buttonType)
{
	bRightMouseDown = false;
	bLeftMouseDown = false;

	ReleaseCapture();
}

void CCamera::OnMouseMove(int X, int Y, string buttonType)
{
	float xRadians = XMConvertToRadians((float)(X - LastMousePosition.x));
	float yRadians = XMConvertToRadians((float)(Y - LastMousePosition.y));
	if (bRightMouseDown )
	{
		RotateAroundPitchAxis(yRadians * MouseSensitity);
		RotateAroundYawAxis(xRadians * MouseSensitity);
	}
	else if (bLeftMouseDown)
	{
		horizontalAngle += yRadians * MouseSensitity;
		verticalAngle += xRadians * MouseSensitity;
	}

	LastMousePosition.x = X;
	LastMousePosition.y = Y;
}

void CCamera::MoveForward(float InValue)
{
	XMFLOAT3& newPos = TransformationComponent->GetPosition();
	XMFLOAT3& forward = TransformationComponent->GetForward();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * KeyboardSensitity);
	XMVECTOR Forward = XMLoadFloat3(&forward);
	XMVECTOR Position = XMLoadFloat3(&newPos);

	XMStoreFloat3(&newPos, XMVectorMultiplyAdd(AmountMovement, Forward, Position));//从Positon点沿着Forward方向移动AmountMovement距离
}

void CCamera::MoveRight(float InValue)
{
	XMFLOAT3& newPos = TransformationComponent->GetPosition();
	XMFLOAT3& right = TransformationComponent->GetRight();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * KeyboardSensitity);
	XMVECTOR Right = XMLoadFloat3(&right);
	XMVECTOR Position = XMLoadFloat3(&newPos);
	XMStoreFloat3(&newPos, XMVectorMultiplyAdd(AmountMovement, Right, Position));//从Positon点沿着Forward方向移动AmountMovement距离
}

void CCamera::MoveUp(float InValue)
{
	XMFLOAT3& newPos = TransformationComponent->GetPosition();
	XMFLOAT3& up = TransformationComponent->GetUp();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * KeyboardSensitity);
	XMVECTOR Up = XMLoadFloat3(&up);
	XMVECTOR Position = XMLoadFloat3(&newPos);
	XMStoreFloat3(&newPos, XMVectorMultiplyAdd(AmountMovement, Up, Position));//从Positon点沿着Forward方向移动AmountMovement距离
}

void CCamera::RotateAroundPitchAxis(float InRotateDegrees)
{
	/*
	* 注意：摄像机在旋转的时候，Pitch方向选转->>>>>始终围绕的摄像机的局部坐标系中的Pitch方向
	*/

	XMFLOAT3 pitch = TransformationComponent->GetRight();
	XMFLOAT3 yaw = TransformationComponent->GetUp();
	XMFLOAT3 roll = TransformationComponent->GetForward();
	

	XMVECTOR PitchAxis = XMLoadFloat3(&pitch);
	XMMATRIX RotatePitchMatrix = XMMatrixRotationAxis(PitchAxis, InRotateDegrees);

	XMStoreFloat3(&TransformationComponent->GetUp(), XMVector3TransformNormal(XMLoadFloat3(&yaw), RotatePitchMatrix));
	XMStoreFloat3(&TransformationComponent->GetForward(), XMVector3TransformNormal(XMLoadFloat3(&roll), RotatePitchMatrix));
}

void CCamera::RotateAroundYawAxis(float InRotateDegrees)
{
	/*
	* 注意：摄像机在旋转的时候，Yaw方向选转->>>>>始终围绕的世界坐标系中的Yaw方向
	*/

	XMFLOAT3 pitch = TransformationComponent->GetRight();
	XMFLOAT3 roll = TransformationComponent->GetForward();

	XMMATRIX RotateYawMatrix = XMMatrixRotationZ(InRotateDegrees);

	XMStoreFloat3(&TransformationComponent->GetRight(), XMVector3TransformNormal(XMLoadFloat3(&pitch), RotateYawMatrix));
	XMStoreFloat3(&TransformationComponent->GetForward(), XMVector3TransformNormal(XMLoadFloat3(&roll), RotateYawMatrix));
}
