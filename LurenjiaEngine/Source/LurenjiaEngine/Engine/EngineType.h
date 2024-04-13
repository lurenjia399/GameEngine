#pragma once
#include "LurenjiaEngine.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Actor/Core/Actor.h"
#include "Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryDescData.h"

struct FHitResult
{
	bool bHit; // �Ƿ�����

	XMFLOAT3 Location;//���������λ��
	XMFLOAT3 ImpactPoint;//��������Ļ��е��λ��

	XMFLOAT3 Normal;//���е��ϵķ��ߣ���ֱ������

	float Distance; // ���ߴ���ʼ�㵽���е�ľ���
	float Time;// ���ߴ���ʼ�㵽���е㾭����ʱ��

	weak_ptr<CMeshComponent> Component_; // ���е�Component���ĸ�
	weak_ptr<AActor> Actor_;//���е�actor���ĸ�

	weak_ptr<FGeometryDescData> GeometryDescData; // ����mesh����Ⱦ����

	FHitResult();
};