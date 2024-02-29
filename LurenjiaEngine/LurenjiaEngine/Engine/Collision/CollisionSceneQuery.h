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
		const XMVECTOR& OriginPoint,		//�������
		const XMVECTOR& Direction,			//���߷���
		const XMMATRIX& ViewInverseMatrix,	//������������
		FHitResult& HitResult);				//���ػ��н��
};


#endif // !Collisionscenequery_h

