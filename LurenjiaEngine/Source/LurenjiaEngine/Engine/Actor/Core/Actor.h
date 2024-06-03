#pragma once

#ifndef Actor
#define Actor
#include "../../LurenjiaEngine.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Component/TransformationComponent.h"

class AActor : public CCoreMinimalObject
{
public:
	AActor();

	virtual void SetPosition(const XMFLOAT3& InPosition);
	virtual void SetRotation(const fvector_3d& InRotation);
	virtual void SetScale(const XMFLOAT3& InScale);

	virtual XMFLOAT3 GetPosition() const;
	virtual fvector_3d GetRotation() const;
	virtual XMFLOAT3 GetScale() const;

	virtual void SetForward(const XMFLOAT3& Forward) const;
	virtual void SetRight(const XMFLOAT3& Right) const;
	virtual void SetUp(const XMFLOAT3& Up) const;

	virtual XMFLOAT3 GetForward() const;
	virtual XMFLOAT3 GetRight() const;
	virtual XMFLOAT3 GetUp() const;

	virtual FORCEINLINE shared_ptr<CTransformationComponent> GetRootComponent() { return RootComponent; }

	virtual void SetPickup(bool InPickup);

	virtual void SetVisible(bool InVisible);
	virtual bool GetVisible();
protected:
	CVARIABLE()
	shared_ptr<CTransformationComponent> RootComponent;
};

#endif