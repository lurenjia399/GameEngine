#pragma once

#ifndef CFogComponent_H
#define CFogComponent_H

#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../Core/Component.h"

class CFogComponent : public CComponent
{
public:
	CFogComponent();

	//外部设置接口
public:
	void SetFogColor(const fvector_color& FogColor);
	void SetFogStart(const float& FogStart);
	void SetFogRange(const float& FogRange);
	void SetDirtyState(const bool& DirtyState);
	void SetFogHeight(const float& FogHeight);
	void SetFogTransparentCoefficient(const float& FogTransparentCoefficient);
	//外部获取接口
public:
	FORCEINLINE XMFLOAT3 GetFogColor() const { return XMFLOAT3(FogColor.r, FogColor.g, FogColor.b); }
	FORCEINLINE float GetFogStart() const { return FogStart; }
	FORCEINLINE float GetFogRange() const { return FogRange; }
	FORCEINLINE float GetFogHeight() const { return FogHeight; }
	FORCEINLINE float GetFogTransparentCoefficient() const { return FogTransparentCoefficient; }
	FORCEINLINE bool GetDirtyState() const { return bDirty; }


private:
	fvector_color FogColor;				//雾的颜色
	float FogStart;						//雾的开始距离
	float FogRange;						//雾的范围
	float FogHeight;					//雾的高度
	float FogTransparentCoefficient;	//雾的透明系数
	bool bDirty;						//更改标志位
};


#endif // !CFogComponent_H



