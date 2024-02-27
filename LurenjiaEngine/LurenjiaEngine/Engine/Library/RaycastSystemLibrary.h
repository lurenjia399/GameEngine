#pragma once
#include "../LurenjiaEngine.h"
#include "../Core/World.h"
#include "../EngineType.h"

struct FRaycastSystemLibrary
{
	static bool HitResultByScreen(CWorld* InWorld, int ScreenX, int ScreenY, FHitResult& OutHitResult);
};
