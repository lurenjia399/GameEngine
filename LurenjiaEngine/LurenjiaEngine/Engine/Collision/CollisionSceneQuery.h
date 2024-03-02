#pragma once

#ifndef COLLISIONSCENEQUERY_H
#define COLLISIONSCENEQUERY_H

#include "../LurenjiaEngine.h"
#include "../EngineType.h"
#include "../Core/World.h"

struct FCollisionSceneQuery
{
	static bool RaycastSingle(
		class CWorld* InWorld,				//����
		const XMVECTOR& ViewOriginPoint,		//�������
		const XMVECTOR& ViewDirection,			//���߷���
		const XMMATRIX& World2ViewMatrixInverse,	//������������
		FHitResult& OutHitResult);				//���ػ��н��
};


#endif // !Collisionscenequery_h

