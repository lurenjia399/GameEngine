#pragma once

#ifndef AFog_H
#define AFog_H

#include "../Core/Actor.h"

class CFogComponent;
class AFog : public AActor
{
public:
	AFog();

	//外部设置接口
public:
	void SetFogColor(const fvector_color& FogColor);
	void SetFogStart(const float& FogStart);
	void SetFogRange(const float& FogRange);
	void SetFogHeight(const float& FogHeight);
	void SetFogTransparentCoefficient(const float& FogTransparentCoefficient);
	void SetDirtyState(const bool& DirtyState);
	void SetFogIsValid(const bool& bValid);
	CFogComponent* GetComponent() const;
	bool GetFogIsValid() const;
private:
	CVARIABLE()
	CFogComponent* FogComponent;
};



#endif
