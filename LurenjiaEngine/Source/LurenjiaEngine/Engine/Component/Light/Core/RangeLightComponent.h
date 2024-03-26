#pragma once
#include "../../TransformationComponent.h"

#ifndef RangeLightComponent_H
#define RangeLightComponent_H

#include "../../../Component/Light/Core/LightType.h"
#include "LightComponent.h"

class CRangeLightComponent : public CLightComponent
{
	typedef CLightComponent super;
public:
	CRangeLightComponent();

	void SetStartAttenuation(float InStartAttenuation);
	void SetEndAttenuation(float InEndAttenuation);

public:
	float GetStartAttenuation();
	float GetEndAttenuation();

private:
	float StartAttenuation;
	float EndAttenuation;
};

#endif // !CLightComponent
