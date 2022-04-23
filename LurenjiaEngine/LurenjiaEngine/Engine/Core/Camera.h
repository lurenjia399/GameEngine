#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"



struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class CCamera : public CCoreMinimalObject, public FViewport
{
public:
	CCamera();
public:
	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

	void BulidViewMatrix(float DeltaTime);
public:
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }
private:
	//代理的执行函数
	void ExecuteKeyboard(const FInputKey& InputKey);
	void OnMouseButtonDown(int X, int Y);
	void OnMouseButtonUp(int X, int Y);
	void OnMouseMove(int X, int Y);

	void MoveForward(float InValue);
	void MoveRight(float InValue);
private:
	CVARIABLE()
	CTransformationComponent* TransformationComponent;
	CVARIABLE()
	CInputComponent* InputComponent;
private:
	POINT LastMousePosition;
};