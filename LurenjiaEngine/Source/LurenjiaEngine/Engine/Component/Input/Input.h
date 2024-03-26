#pragma once
#include "../../Core/Engine.h"

DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesDelegate, void, int, int, string);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelDelegate, void, int, int, float);

extern FCaptureOnMousesDelegate MouseDownDelegate;
extern FCaptureOnMousesDelegate MouseUpDelegate;
extern FCaptureOnMousesDelegate MouseMoveDelegate;
extern FCaptureOnMousesWheelDelegate MouseWheelDelegate;