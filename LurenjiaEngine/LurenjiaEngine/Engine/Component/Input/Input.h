#pragma once
#include "../../Core/Engine.h"

DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesDelegate, void, int, int);

extern FCaptureOnMousesDelegate MouseDownDelegate;
extern FCaptureOnMousesDelegate MouseUpDelegate;
extern FCaptureOnMousesDelegate MouseMoveDelegate;