#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../Interface/DirectXDeviceInterface.h"
#include "../Actor/Core/Actor.h"
#include "Viewport/ClientViewport.h"


DEFINITION_MULTICAST_SINGLE_DELEGATE(FSelectedActorDelegate, void, bool);

enum ECameraType;
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class ACamera : public AClientViewport, public IDirectXDeviceInterface
{
	typedef AClientViewport Super;

public:
	ACamera();
public:
	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

public:
	// 鼠标点击
	void OnClickedScreen(int X, int Y);
	
public:
	FORCEINLINE shared_ptr<CInputComponent> GetInputComponent() { return InputComponent; }
public:
	FSelectedActorDelegate SelectedActorDelegate; // 选中Actor的代理
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
	shared_ptr<CInputComponent> InputComponent;
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