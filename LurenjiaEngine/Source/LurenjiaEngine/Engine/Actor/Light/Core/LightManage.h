#pragma once

#ifndef LightManageH
#define LightManageH

#include "CoreObject/CoreMinimalObject.h"

class CLightComponent;
class CLightManage : public CCoreMinimalObject
{
	friend struct FGeometry;
	friend struct FGeometryMap;
public:

	void AddLightComponent(CLightComponent* InLightComponent);
private:
	vector<CLightComponent*> Lights;
};






#endif