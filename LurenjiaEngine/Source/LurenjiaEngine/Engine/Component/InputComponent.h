#pragma once
#include "Core/Component.h"
#include "Input/InputType.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInforDelegate, void, const FInputKey&);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int, string);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseWheelDelegate, void, int, int, float);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnLMouseButtonDownDelegate, void, int, int);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnLMouseButtonUpDelegate, void, int, int);


class CInputComponent : public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyboardInforDelegate CaptureKeyboardInforDelegate;

	CVARIABLE()
	FCaptureOnMouseDelegate OnMouseButtonDownDelegate;

	CVARIABLE()
	FCaptureOnMouseDelegate OnMouseButtonUpDelegate;

	CVARIABLE()
	FCaptureOnMouseDelegate OnMouseMoveDelegate;

	CVARIABLE()
	FCaptureOnMouseWheelDelegate OnMouseWheelDelegate;

	CVARIABLE()
	FCaptureOnLMouseButtonDownDelegate OnLMouseButtonDownDelegate;

	CVARIABLE()
	FCaptureOnLMouseButtonUpDelegate OnLMouseButtonUpDelegate;

public:
	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

private:
	void OnMouseButtonDown(int X, int Y, string buttonType);
	void OnMouseButtonUp(int X, int Y, string buttonType);
	void OnMouseMove(int X, int Y, string buttonType);
	void OnMouseWheel(int X, int Y, float InValue);
	void OnLMouseButtonDown(int X, int Y);
	void OnLMouseButtonUp(int X, int Y);
};