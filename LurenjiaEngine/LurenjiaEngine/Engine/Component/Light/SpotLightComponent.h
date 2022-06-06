#pragma once


#ifndef SpotLightComponent_H
#define SpotLightComponent_H

#include "Core/LightComponent.h"
#include "../../Component/Mesh/Core/MeshComponent.h"

class CSpotLightComponent : public CLightComponent
{
	typedef CLightComponent super;
public:
	CSpotLightComponent();
public:
	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;

	void SetStartAttenuation(float InStartAttenuation);
	void SetEndAttenuation(float InEndAttenuation);
public:
	float GetStartAttenuation();
	float GetEndAttenuation();
private:
	CMeshComponent* LightMeshComponent;
	float StartAttenuation;
	float EndAttenuation;
};

#endif