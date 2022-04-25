#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../Interface/DirectXDeviceInterface.h"


struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class CCamera : public CCoreMinimalObject, public FViewport, public IDirectXDeviceInterface
{
public:
	CCamera();
public:
	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

	
public:
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }
private:
	//代理的执行函数
	void ExecuteKeyboard(const FInputKey& InputKey);
	void OnMouseButtonDown(int X, int Y, string buttonType);
	void OnMouseButtonUp(int X, int Y, string buttonType);
	void OnMouseMove(int X, int Y, string buttonType);

	void MoveForward(float InValue);
	void MoveRight(float InValue);
	void MoveUp(float InValue);

private:
	void RotateAroundYawAxis(float InRotateDegrees);
	void RotateAroundPitchAxis(float InRotateDegrees);

	void BulidViewMatrix(float DeltaTime);
	void FocusMeshUpdateViewMatrix();
private:
	CVARIABLE()
	CTransformationComponent* TransformationComponent;
	CVARIABLE()
	CInputComponent* InputComponent;
private:
	POINT LastMousePosition;
	bool bRightMouseDown = false;
	bool bLeftMouseDown = false;
	bool bFoucsMode = false;

	float MouseSensitity = 0.5;
	float KeyboardSensitity = 1.3;
private:
	float Radius;			//球面半径
	float horizontalAngle;	//水平角
	float verticalAngle;	//竖直角
};