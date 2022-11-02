#pragma once

#ifndef AClientViewport_H
#define AClientViewport_H

#include "../../Actor/Core/Actor.h"
#include "../../Core/Viewport/Viewport.h"

class AClientViewport : public AActor, public FViewport
{
	typedef AActor SuperA;
	typedef FViewport SuperV;


public:
	virtual void BulidViewMatrix(float DeltaTime);
	virtual void Tick(float DeltaTime) override;
public:
	AClientViewport();
	// …Ë÷√ ”◊∂
	void SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar);
	void FaceTarget(const XMFLOAT3& InPosition, const XMFLOAT3& InTargetPosition, const XMFLOAT3& InUpDirection = XMFLOAT3(0.f, 0.f, 1.f));


	FORCEINLINE float GetFOV() const { return YFOV; }
	FORCEINLINE float GetAspect() const { return Aspect; }
	FORCEINLINE float GetNear() const { return ZNear; }
	FORCEINLINE float GetFar() const { return ZFar; }
	FORCEINLINE float GetDirtyState() const { return bDirty; }

	void SetDirty(bool bNewDirty) { bDirty = bNewDirty; }


private:
	float YFOV;
	float Aspect;
	float ZNear;
	float ZFar;

	bool bDirty;
};


#endif // !