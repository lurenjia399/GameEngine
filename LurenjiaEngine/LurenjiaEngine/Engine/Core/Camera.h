#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../Interface/DirectXDeviceInterface.h"
#include "../Actor/Core/Actor.h"

enum ECameraType;
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class ACamera : public AActor, public FViewport, public IDirectXDeviceInterface
{
public:
	ACamera();
public:
	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

	
public:
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
private:
	//代理的执行函数
	void ExecuteKeyboard(const FInputKey& InputKey);
	void OnMouseButtonDown(int X, int Y, string buttonType);
	void OnMouseButtonUp(int X, int Y, string buttonType);
	void OnMouseMove(int X, int Y, string buttonType);
	void OnMouseWheel(int X, int Y, float InValue);

	void MoveForward(float InValue);
	void MoveRight(float InValue);
	void MoveUp(float InValue);

private:
	void RotateAroundYawAxis(float InRotateDegrees);
	void RotateAroundPitchAxis(float InRotateDegrees);

	void BulidViewMatrix(float DeltaTime);
	void FocusMeshUpdateCameraInfo(float InValue);
private:
	CVARIABLE()
	CInputComponent* InputComponent;
private:
	POINT LastMousePosition = {};
	bool bRightMouseDown = false;
	bool bLeftMouseDown = false;
	bool bFoucsMode = false;

	float MouseSensitity = 0.5f;
	float KeyboardSensitity = 1.3f;
private:
	float Radius;			//球面半径
	float horizontalAngle;	//水平角
	float verticalAngle;	//竖直角
	ECameraType CameraType;	//暂时的摄像机模式
};