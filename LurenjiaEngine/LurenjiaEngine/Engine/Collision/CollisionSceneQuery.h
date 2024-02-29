#pragma once

#ifndef COLLISIONSCENEQUERY_H
#define COLLISIONSCENEQUERY_H

#include "../LurenjiaEngine.h"
#include "../EngineType.h"
#include "../Core/World.h"

struct FCollisionSceneQuery
{
	static bool RaycastSingle(
		class CWorld* InWorld,				//世界
		const XMVECTOR& OriginPoint,		//射线起点
		const XMVECTOR& Direction,			//射线方向
		const XMMATRIX& ViewInverseMatrix,	//摄像机矩阵的逆
		FHitResult& HitResult);				//返回击中结果
};


#endif // !Collisionscenequery_h

