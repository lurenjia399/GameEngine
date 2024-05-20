#pragma once
#include "../LurenjiaEngine.h"
#include "../Core/World.h"
#include "../EngineType.h"

struct FRaycastSystemLibrary
{
	static bool HitResultByScreen(shared_ptr<CWorld> InWorld, int ScreenX, int ScreenY, FHitResult& OutHitResult);
	static bool HitSpecificObjectsResultByScreen(shared_ptr<CWorld> InWorld, AActor* InSpecificObject, int ScreenX, int ScreenY, FHitResult& OutHitResult);
};
