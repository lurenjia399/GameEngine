#include "InputComponent.h"
#include "Input/Input.h"

void CInputComponent::BeginInit()
{
	MouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseButtonDown);
	MouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseButtonUp);
	MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMove);
}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyboardInforDelegate.IsBound())
	{
		FInputKey InputKey;
		if (GetAsyncKeyState('W') & 0x8000)
		{
			InputKey.KeyName = "W";
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			InputKey.KeyName = "S";
		}
		else if (GetAsyncKeyState('A') & 0x8000)
		{
			InputKey.KeyName = "A";
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			InputKey.KeyName = "D";
		}
		else {
			return;
		}
		CaptureKeyboardInforDelegate.Execute(InputKey);
	}
}

void CInputComponent::OnMouseButtonDown(int X, int Y)
{
	if (OnMouseButtonDownDelegate.IsBound())
	{
		OnMouseButtonDownDelegate.Execute(X, Y);
	}
}

void CInputComponent::OnMouseButtonUp(int X, int Y)
{
	if (OnMouseButtonUpDelegate.IsBound())
	{
		OnMouseButtonUpDelegate.Execute(X, Y);
	}
}

void CInputComponent::OnMouseMove(int X, int Y)
{
	if (OnMouseMoveDelegate.IsBound())
	{
		OnMouseMoveDelegate.Execute(X, Y);
	}
}
