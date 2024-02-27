#pragma once
#include "LurenjiaEngine.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Actor/Core/Actor.h"

struct FHitResult
{
	bool bHit; // 是否命中

	XMFLOAT3 Location;//命中物体的位置
	XMFLOAT3 ImpactPoint;//命中物体的击中点的位置

	XMFLOAT3 Normal;//击中点上的法线，垂直于射线

	float Distance; // 射线从起始点到击中点的距离
	float Time;// 射线从起始点到击中点经过的事件

	shared_ptr<CMeshComponent> Component_; // 击中的Component是哪个
	shared_ptr<AActor> Actor_;//击中的actor是哪个

	FHitResult();
};