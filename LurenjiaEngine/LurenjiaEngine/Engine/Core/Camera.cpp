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
	InputComponent->CaptureKeyboardInforDelegate.Bind(this, &CCamera::ExecuteKeyboard);
}

void CCamera::Tick(float DeltaTime)
{
}

void CCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
}
