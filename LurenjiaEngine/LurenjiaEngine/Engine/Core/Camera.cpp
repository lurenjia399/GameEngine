#include "Camera.h"
#include "../Component/TransformationComponent.h"
#include "../Component/InputComponent.h"
#include "CameraType.h"

CCamera::CCamera()
{
	InputComponent = CreateObject<CInputComponent>("InputComponent");
	TransformationComponent = CreateObject<CTransformationComponent>("TransformationComponent");

	Radius = 20.0f;
	verticalAngle = XM_PI/2;
	horizontalAngle = XM_PI/2;
	CameraType = ECameraType::CameraRoaming;
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
	if (CameraType == ECameraType::ObservationObject && bLeftMouseDown)
	{
		XMFLOAT3 MeshPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT3 cameraForward = {};
		XMFLOAT3 cameraPosition = {};
		XMFLOAT3 cameraUp = {};

		cameraPosition.x = Radius * sinf(verticalAngle) * cosf(horizontalAngle);
		cameraPosition.z = Radius * cosf(verticalAngle);
		cameraPosition.y = Radius * sinf(verticalAngle) * sinf(horizontalAngle);

		cameraForward.x = MeshPosition.x - cameraPosition.x;
		cameraForward.y = MeshPosition.y - cameraPosition.y;
		cameraForward.z = MeshPosition.z - cameraPosition.z;

		//归一化摄像机三向量
		XMVECTOR forward = XMLoadFloat3(&cameraForward);
		forward = XMVector3Normalize(forward);
		XMFLOAT3 WordUpfloat3 = XMFLOAT3(0, 0, 1);
		XMVECTOR WordUp = XMLoadFloat3(&WordUpfloat3);
		XMVECTOR right = XMVector3Cross(WordUp, forward);
		right = XMVector3Normalize(right);
		WordUp = XMVector3Cross(forward, right);

		XMMATRIX rotateMatrix =
		{
			right.m128_f32[0],	WordUp.m128_f32[0], forward.m128_f32[0],	0.f,
			right.m128_f32[1],	WordUp.m128_f32[1], forward.m128_f32[1],	0.f,
			right.m128_f32[2],	WordUp.m128_f32[2], forward.m128_f32[2],	0.f,
			0,					0,					0,						1.f
		};
		rotateMatrix = XMMatrixTranspose(rotateMatrix);
		XMMATRIX TranslateMatrix =
		{
			1.f, 0.f, 0.f, -cameraPosition.x,
			0.f, 1.f, 0.f, -cameraPosition.y,
			0.f, 0.f, 1.f, -cameraPosition.z,
			0.f, 0.f, 0.f, 1.f
		};
		XMStoreFloat4x4(&ViewMatrix, rotateMatrix * TranslateMatrix);
	}
	else if (CameraType == ECameraType::CameraRoaming)
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
}

void CCamera::FocusMeshUpdateCameraInfo(float InValue)
{
	XMFLOAT3 MeshPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	if (InValue > 0)	//进入观察模式
	{
		CameraType = ECameraType::ObservationObject;

		XMFLOAT3 cameraForward = TransformationComponent->GetForward();
		XMFLOAT3 cameraPosition = TransformationComponent->GetPosition();

		cameraPosition.x = Radius * sinf(verticalAngle) * cosf(horizontalAngle);
		cameraPosition.z = Radius * cosf(verticalAngle);
		cameraPosition.y = Radius * sinf(verticalAngle) * sinf(horizontalAngle);

		cameraForward.x = MeshPosition.x - cameraPosition.x;
		cameraForward.y = MeshPosition.y - cameraPosition.y;
		cameraForward.z = MeshPosition.z - cameraPosition.z;

		//归一化摄像机三向量
		XMFLOAT3 WordUpfloat3 = XMFLOAT3(0, 0, 1);
		XMVECTOR WordUp = XMLoadFloat3(&WordUpfloat3);
		XMVECTOR right = XMVector3Cross(WordUp, XMLoadFloat3(&cameraForward));
		right = XMVector3Normalize(right);
		XMVECTOR forward = XMVector3Cross(right, WordUp);

		XMMATRIX rotateMatrix =
		{
			right.m128_f32[0],	WordUp.m128_f32[0], forward.m128_f32[0],	0.f,
			right.m128_f32[1],	WordUp.m128_f32[1], forward.m128_f32[1],	0.f,
			right.m128_f32[2],	WordUp.m128_f32[2], forward.m128_f32[2],	0.f,
			0,				0,			0,					1.f
		};
		rotateMatrix = XMMatrixTranspose(rotateMatrix);
		XMMATRIX TranslateMatrix =
		{
			1.f, 0.f, 0.f, -cameraPosition.x,
			0.f, 1.f, 0.f, -cameraPosition.y,
			0.f, 0.f, 1.f, -cameraPosition.z,
			0.f, 0.f, 0.f, 1.f
		};
		XMStoreFloat4x4(&ViewMatrix, rotateMatrix * TranslateMatrix);
	}
	else {	//恢复漫游模式
		CameraType = ECameraType::CameraRoaming;
	}
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
	if (InputKey.KeyName == "R")
	{
		FocusMeshUpdateCameraInfo(1.0f);
	}
	if (InputKey.KeyName == "F")
	{
		FocusMeshUpdateCameraInfo(-1.0f);
	}
}

void CCamera::OnMouseButtonDown(int X, int Y, string buttonType)
{
	if (buttonType == "R" && CameraType != ECameraType::ObservationObject)
	{
		bRightMouseDown = true;
	}
	else if (buttonType == "L" && CameraType != ECameraType::CameraRoaming)
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
		//horizontalAngle += xRadians * MouseSensitity;
		verticalAngle += yRadians * MouseSensitity;
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
