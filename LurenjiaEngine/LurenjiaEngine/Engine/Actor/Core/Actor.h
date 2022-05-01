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
	void SetPosition(const XMFLOAT3& InPosition);
	void SetRotation(const fvector_3d& InRotation);
	void SetScale(const XMFLOAT3& InScale);

	XMFLOAT3 GetPosition() const;
	fvector_3d GetRotation() const;
	XMFLOAT3 GetScale() const;
	XMFLOAT3 GetForward() const;
	
	XMFLOAT3 GetRight() const;
	XMFLOAT3 GetUp() const;

	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }
protected:
	CVARIABLE()
	CTransformationComponent* TransformationComponent;
};

#endif