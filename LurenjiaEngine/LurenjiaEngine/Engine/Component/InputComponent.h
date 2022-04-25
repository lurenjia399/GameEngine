#pragma once
#include "Core/Component.h"
#include "Input/InputType.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInforDelegate, void, const FInputKey&);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int, string);

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

public:
	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

	void OnMouseButtonDown(int X, int Y, string buttonType);
	void OnMouseButtonUp(int X, int Y, string buttonType);
	void OnMouseMove(int X, int Y, string buttonType);
};