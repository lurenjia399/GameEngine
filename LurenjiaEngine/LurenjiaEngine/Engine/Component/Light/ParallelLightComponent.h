#pragma once


#ifndef ParallelLightComponent_H
#define ParallelLightComponent_H

#include "Core/LightComponent.h"
#include "../../Component/Mesh/Core/MeshComponent.h"

class CParallelLightComponent : public CLightComponent
{
	typedef CLightComponent super;
public:
	CParallelLightComponent();

	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;
private:
	std::shared_ptr<CMeshComponent> LightMeshComponent;
};

#endif