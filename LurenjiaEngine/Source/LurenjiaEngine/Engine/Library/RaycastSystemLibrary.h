#pragma once
#include "../LurenjiaEngine.h"
#include "../Core/World.h"
#include "../EngineType.h"

struct FRaycastSystemLibrary
{
	static bool GetRaycastByScreenParam(CWorld* InWorld, XMVECTOR& OutViewOriginPoint,
		XMVECTOR& OutViewDirection,
		XMMATRIX& OutWorld2ViewMatrixInverse, int ScreenX, int ScreenY);
	static bool HitResultByScreen(CWorld* InWorld, int ScreenX, int ScreenY, FHitResult& OutHitResult);
	static bool HitSpecificObjectsResultByScreen(CWorld* InWorld, AActor* InSpecificObject, int ScreenX, int ScreenY, FHitResult& OutHitResult);
};
