#pragma once

#ifndef CFogComponent_H
#define CFogComponent_H

#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../Core/Component.h"

class CFogComponent : public CComponent
{
public:
	CFogComponent();

	//�ⲿ���ýӿ�
public:
	void SetFogColor(const fvector_color& FogColor);
	void SetFogStart(const float& FogStart);
	void SetFogRange(const float& FogRange);
	void SetDirtyState(const bool& DirtyState);
	void SetFogHeight(const float& FogHeight);
	//�ⲿ��ȡ�ӿ�
public:
	FORCEINLINE XMFLOAT3 GetFogColor() const { return XMFLOAT3(FogColor.r, FogColor.g, FogColor.b); }
	FORCEINLINE float GetFogStart() const { return FogStart; }
	FORCEINLINE float GetFogRange() const { return FogRange; }
	FORCEINLINE float GetFogHeight() const { return FogHeight; }
	FORCEINLINE bool GetDirtyState() const { return bDirty; }


private:
	fvector_color FogColor; //�����ɫ
	float FogStart;	//��Ŀ�ʼ����
	float FogRange;	//��ķ�Χ
	float FogHeight; //��ĸ߶�
	bool bDirty; //���ı�־λ
};


#endif // !CFogComponent_H



