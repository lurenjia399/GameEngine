#pragma once


#ifndef SpotLightComponent_H
#define SpotLightComponent_H

#include "Core/RangeLightComponent.h"
#include "../../Component/Mesh/Core/MeshComponent.h"

class CSpotLightComponent : public CRangeLightComponent
{
	typedef CLightComponent super;
public:
	CSpotLightComponent();
public:
	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;

	void SetConicalInnerCorner(float InConicalInnerCorner);
	void SetConicalOuterCorner(float InConicalOuterCorner);

public:
	float GetConicalInnerCorner() const;
	float GetConicalOuterCorner() const;
	
private:
	CMeshComponent* LightMeshComponent;
	float		ConicalInnerCorner;     //�۹��׶���ڽǣ��Ƕ�
	float		ConicalOuterCorner;     //�۹��׶����ǣ��Ƕ�
};

#endif