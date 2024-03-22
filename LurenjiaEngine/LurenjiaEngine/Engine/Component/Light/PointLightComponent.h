#pragma once


#ifndef PointLightComponent_H
#define PointLightComponent_H

#include "Core/RangeLightComponent.h"
#include "../../Component/Mesh/Core/MeshComponent.h"

class CPointLightComponent : public CRangeLightComponent
{
	typedef CRangeLightComponent super;
public:
	CPointLightComponent();
public:
	void SetPosition(const XMFLOAT3& InPosition) override;
	void SetRotation(const fvector_3d& InRotation) override;
	void SetScale(const XMFLOAT3& InScale) override;

	template<typename T>
	std::shared_ptr<T> GetLightMeshComponent()
	{
		if (LightMeshComponent)
		{
			return static_pointer_cast<T>(LightMeshComponent);
		}
		return nullptr;
	}
private:
	std::shared_ptr<CMeshComponent> LightMeshComponent;
};

#endif