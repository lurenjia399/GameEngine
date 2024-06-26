#include "Camera.h"
#include "../Component/TransformationComponent.h"
#include "../Component/InputComponent.h"
#include "CameraType.h"
#include "../Config/EngineRenderConfig.h"
#include "../Library/RaycastSystemLibrary.h"
#include "../EngineType.h"
#include "../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/RenderingLayer/RenderLayerManage.h"
#include "../Editor/SelectEditor/OperationHandle/Core/OperationHandleBase.h"

ACamera::ACamera()
{
	InputComponent = LurenjiaEngine::CreateObject<CInputComponent>(this, "CameraInputComponent");

	Radius = 20.0f;
	verticalAngle = XM_PI / 2;
	horizontalAngle = XM_PI / 2;
	CameraType = ECameraType::CameraRoaming;
}

void ACamera::BeginInit()
{
	Super::BeginInit();
	

	ViewportInit();	//初始化p矩阵
	InputComponent->CaptureKeyboardInforDelegate.Bind(this, &ACamera::ExecuteKeyboard);
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &ACamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &ACamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &ACamera::OnMouseMove);
	InputComponent->OnMouseWheelDelegate.Bind(this, &ACamera::OnMouseWheel);
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACamera::OnClickedScreen(int X, int Y)
{
	if (AOperationHandleBase::SelectedAxisComponent.lock())
	{
		// 如果当前选中了操作轴，就不让选择物体了
		return;
	}

	shared_ptr<CDirectXRenderingEngine> DXRenderingEngine = static_pointer_cast<CDirectXRenderingEngine>(GetRenderEngine());
	if (DXRenderingEngine != nullptr)
	{
		FHitResult HitResult = {};
		bool bHit = FRaycastSystemLibrary::HitResultByScreen(DXRenderingEngine->GetWorld(), X, Y, HitResult);
		if (bHit)
		{
			if (!HitResult.Component_.expired())
			{
				std::shared_ptr<CMeshComponent> component = HitResult.Component_.lock();
				Engine_Log_Success("HitResult name[%s]", component->GetName().c_str());

				FRenderLayerManage::GetRenderLayerManage()->ClearGeometryDescData((int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT);
				FRenderLayerManage::GetRenderLayerManage()->AddGeometryDescData((int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT, HitResult.GeometryDescData);
				AOperationHandleBase::SelectedActor = HitResult.Actor_;
				
			}
		}
		else
		{
			FRenderLayerManage::GetRenderLayerManage()->ClearGeometryDescData((int)EMeshComponentRenderLayerType::RENDERLAYER_SELECT);
			AOperationHandleBase::SelectedActor.reset();
			Engine_Log("No Hit Actor");
		}

		SelectedActorDelegate.Broadcast(bHit);
	}
	
}

void ACamera::BulidViewMatrix(float DeltaTime)
{
	if (CameraType == ECameraType::ObservationObject )//&& bLeftMouseDown)
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

		int per = (int)floor(verticalAngle / XM_PI);
		XMFLOAT3 WordUpfloat3 = XMFLOAT3(0, 0, 1);
		if (verticalAngle > 0)
		{
			if (per % 2 == 1)
			{
				WordUpfloat3.z = -1;
			}
		}
		else if (verticalAngle < 0)
		{
			if (per % 2 == 0)
			{
				WordUpfloat3.z = -1;
			}
		}
		XMVECTOR WordUp = XMLoadFloat3(&WordUpfloat3);
		XMVECTOR right = XMVector3Cross(WordUp, forward);
		right = XMVector3Normalize(right);
		if (verticalAngle <= 0.000001)
		{
			XMFLOAT3 Right = XMFLOAT3(1, 0, 0);
			right = XMLoadFloat3(&Right);
		}
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
		Super::BulidViewMatrix(DeltaTime);
	}
}

void ACamera::FocusMeshUpdateCameraInfo(float InValue)
{
	XMFLOAT3 MeshPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	if (InValue > 0)	//进入观察模式
	{
		CameraType = ECameraType::ObservationObject;
		XMFLOAT3 cameraForward = RootComponent->GetForward();
		XMFLOAT3 cameraPosition = RootComponent->GetPosition();

		cameraPosition.x = Radius * sinf(verticalAngle) * cosf(horizontalAngle);
		cameraPosition.z = Radius * cosf(verticalAngle);
		cameraPosition.y = Radius * sinf(verticalAngle) * sinf(horizontalAngle);

		cameraForward.x = MeshPosition.x - cameraPosition.x;
		cameraForward.y = MeshPosition.y - cameraPosition.y;
		cameraForward.z = MeshPosition.z - cameraPosition.z;

		XMFLOAT3 WordUpfloat3 = XMFLOAT3(0, 0, 1);
		XMVECTOR forward = XMLoadFloat3(&cameraForward);
		forward = XMVector3Normalize(forward);
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
	else {	//恢复漫游模式
		CameraType = ECameraType::CameraRoaming;

		Radius = 20.0f;
		verticalAngle = XM_PI / 2;
		horizontalAngle = XM_PI / 2;
	}
}

void ACamera::ExecuteKeyboard(const FInputKey& InputKey)
{
	if (InputKey.KeyName == "R")
	{
		FocusMeshUpdateCameraInfo(1.0f);
		SetDirty(true);
	}
	if (InputKey.KeyName == "F")
	{
		FocusMeshUpdateCameraInfo(-1.0f);
		SetDirty(true);
	}
	if (CameraType == ECameraType::ObservationObject) return;

	if (InputKey.KeyName == "W")
	{
		MoveForward(1.f);
		SetDirty(true);
	}
	if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);
		SetDirty(true);
	}
	if (InputKey.KeyName == "D")
	{
		MoveRight(1.0f);
		SetDirty(true);
	}
	if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);
		SetDirty(true);
	}
	if (InputKey.KeyName == "E")
	{
		MoveUp(1.0f);
		SetDirty(true);
	}
	if (InputKey.KeyName == "Q")
	{
		MoveUp(-1.0f);
		SetDirty(true);
	}
}

void ACamera::OnMouseButtonDown(int X, int Y, string buttonType)
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

	if (buttonType == "L")
	{
		OnClickedScreen(X, Y);
	}

	SetDirty(true);
}

void ACamera::OnMouseButtonUp(int X, int Y, string buttonType)
{
	bRightMouseDown = false;
	bLeftMouseDown = false;

	ReleaseCapture();

	SetDirty(true);
}

void ACamera::OnMouseMove(int X, int Y, string buttonType)
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
		horizontalAngle += xRadians * MouseSensitity;
		verticalAngle += -yRadians * MouseSensitity;

		verticalAngle = math_libray::Clamp(verticalAngle, 0.f, XM_2PI);
		horizontalAngle = math_libray::Clamp(horizontalAngle, 0.f, XM_2PI);
	}

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetDirty(true);
}

void ACamera::OnMouseWheel(int X, int Y, float InValue)
{
	if (CameraType == ECameraType::ObservationObject)
	{
		Radius += InValue / 100.f;
		Radius = math_libray::Clamp(Radius, 5.0f, 40.f);

		SetDirty(true);
	}
}

void ACamera::MoveForward(float InValue)
{
	XMFLOAT3& newPos = RootComponent->GetPosition();
	XMFLOAT3& forward = RootComponent->GetForward();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * KeyboardSensitity);
	XMVECTOR Forward = XMLoadFloat3(&forward);
	XMVECTOR Position = XMLoadFloat3(&newPos);

	XMStoreFloat3(&newPos, XMVectorMultiplyAdd(AmountMovement, Forward, Position));//从Positon点沿着Forward方向移动AmountMovement距离
}

void ACamera::MoveRight(float InValue)
{
	XMFLOAT3& newPos = RootComponent->GetPosition();
	XMFLOAT3& right = RootComponent->GetRight();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * KeyboardSensitity);
	XMVECTOR Right = XMLoadFloat3(&right);
	XMVECTOR Position = XMLoadFloat3(&newPos);
	XMStoreFloat3(&newPos, XMVectorMultiplyAdd(AmountMovement, Right, Position));//从Positon点沿着Right方向移动AmountMovement距离
}

void ACamera::MoveUp(float InValue)
{
	XMFLOAT3& newPos = RootComponent->GetPosition();
	XMFLOAT3& up = RootComponent->GetUp();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * KeyboardSensitity);
	XMVECTOR Up = XMLoadFloat3(&up);
	XMVECTOR Position = XMLoadFloat3(&newPos);
	XMStoreFloat3(&newPos, XMVectorMultiplyAdd(AmountMovement, Up, Position));//从Positon点沿着Up方向移动AmountMovement距离
}

void ACamera::RotateAroundPitchAxis(float InRotateDegrees)
{
	/*
	* 注意：摄像机在旋转的时候，Pitch方向选转->>>>>始终围绕的摄像机的局部坐标系中的Pitch方向
	*/

	XMFLOAT3 pitch = RootComponent->GetRight();
	XMFLOAT3 yaw = RootComponent->GetUp();
	XMFLOAT3 roll = RootComponent->GetForward();
	

	XMVECTOR PitchAxis = XMLoadFloat3(&pitch);
	XMMATRIX RotatePitchMatrix = XMMatrixRotationAxis(PitchAxis, InRotateDegrees);

	XMStoreFloat3(&RootComponent->GetUp(), XMVector3TransformNormal(XMLoadFloat3(&yaw), RotatePitchMatrix));
	XMStoreFloat3(&RootComponent->GetForward(), XMVector3TransformNormal(XMLoadFloat3(&roll), RotatePitchMatrix));
}

void ACamera::RotateAroundYawAxis(float InRotateDegrees)
{
	/*
	* 注意：摄像机在旋转的时候，Yaw方向选转->>>>>始终围绕的世界坐标系中的Yaw方向
	*/

	XMFLOAT3 pitch = RootComponent->GetRight();
	XMFLOAT3 roll = RootComponent->GetForward();

	// 这里绕着z轴旋转已经是UE坐标系了
	XMMATRIX RotateYawMatrix = XMMatrixRotationZ(InRotateDegrees);
	//XMMATRIX RotateYawMatrix = XMMatrixRotationY(InRotateDegrees);

	XMStoreFloat3(&RootComponent->GetRight(), XMVector3TransformNormal(XMLoadFloat3(&pitch), RotateYawMatrix));
	XMStoreFloat3(&RootComponent->GetForward(), XMVector3TransformNormal(XMLoadFloat3(&roll), RotateYawMatrix));
}
