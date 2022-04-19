#include "InputComponent.h"

void CInputComponent::BeginInit()
{
}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyboardInforDelegate.IsBound())
	{
		FInputKey InputKey;
		CaptureKeyboardInforDelegate.Execute(InputKey);
	}
}
