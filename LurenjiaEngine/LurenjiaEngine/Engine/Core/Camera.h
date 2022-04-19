#pragma once
#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"



struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class CCamera : public CCoreMinimalObject, public FViewport
{
public:
	CCamera();
private:
	CVARIABLE()
	CTransformationComponent* TransformationComponent;

	CVARIABLE()
	CInputComponent* InputComponent;
public:
	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;
	virtual void ExecuteKeyboard(const FInputKey& InputKey);
public:
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }
};