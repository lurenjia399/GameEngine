#include "InputComponent.h"
#include "Input/Input.h"

void CInputComponent::BeginInit()
{
	MouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseButtonDown);
	MouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseButtonUp);
	MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMove);
	MouseWheelDelegate.AddFunction(this, &CInputComponent::OnMouseWheel);
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
		else if (GetAsyncKeyState('E') & 0x8000)
		{
			InputKey.KeyName = "E";
		}
		else if (GetAsyncKeyState('Q') & 0x8000)
		{
			InputKey.KeyName = "Q";
		}
		else if (GetAsyncKeyState('R') & 0x8000)
		{
			InputKey.KeyName = "R";			//暂时 进入旋转模型模式
		}
		else if (GetAsyncKeyState('F') & 0x8000)
		{
			InputKey.KeyName = "F";			//暂时 退出旋转模型模式
		}
		else {
			return;
		}
		CaptureKeyboardInforDelegate.Execute(InputKey);
	}
}

void CInputComponent::OnMouseButtonDown(int X, int Y, string buttonType)
{
	if (OnMouseButtonDownDelegate.IsBound())
	{
		OnMouseButtonDownDelegate.Execute(X, Y, buttonType);
	}
}

void CInputComponent::OnMouseButtonUp(int X, int Y, string buttonType)
{
	if (OnMouseButtonUpDelegate.IsBound())
	{
		OnMouseButtonUpDelegate.Execute(X, Y, buttonType);
	}
}

void CInputComponent::OnMouseMove(int X, int Y, string buttonType)
{
	if (OnMouseMoveDelegate.IsBound())
	{
		OnMouseMoveDelegate.Execute(X, Y, buttonType);
	}
}

void CInputComponent::OnMouseWheel(int X, int Y, float InValue)
{
	if (OnMouseWheelDelegate.IsBound())
	{
		OnMouseWheelDelegate.Execute(X, Y, InValue);
	}
}
