#pragma once

#ifndef COLLISIONSCENEQUERY_H
#define COLLISIONSCENEQUERY_H

#include "../LurenjiaEngine.h"
#include "../EngineType.h"
#include "../Core/World.h"

struct FCollisionSceneQuery
{
	static bool RaycastSingle(
		shared_ptr<class CWorld> InWorld,				//����
		const XMVECTOR& ViewOriginPoint,				//�������
		const XMVECTOR& ViewDirection,					//���߷���
		const XMMATRIX& World2ViewMatrixInverse,		//������������
		FHitResult& OutHitResult);						//���ػ��н��

	static bool RaycastSingle(
		shared_ptr<class CWorld> InWorld,				//����
		AActor* SpecificObject,							//�����Actor
		const XMVECTOR& ViewOriginPoint,				//�������
		const XMVECTOR& ViewDirection,					//���߷���
		const XMMATRIX& World2ViewMatrixInverse,		//������������
		FHitResult& OutHitResult);						//���ػ��н��
};


#endif // !Collisionscenequery_h

