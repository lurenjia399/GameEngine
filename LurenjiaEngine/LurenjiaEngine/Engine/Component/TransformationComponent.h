#pragma once
#include "Core/Component.h"

class CTransformationComponent : public CCoreMinimalObject
{
public:
	CTransformationComponent();

	void SetPosition(const XMFLOAT3& InPosition);
	void SetRotation(const fvector_3d& InRotation);
	void SetScale(const XMFLOAT3& InScale);

	void SetForward(const XMFLOAT3& InForward);
	void SetRight(const XMFLOAT3& InRight);
	void SetUp(const XMFLOAT3& InUp);

	XMFLOAT3& GetPosition(){ return Position; }
	fvector_3d GetRotation(){ return fvector_3d(Rotation.x, Rotation.y, Rotation.z); }
	XMFLOAT3& GetScale() { return Scale; }
	XMFLOAT3& GetForward(){ return ForwardVector; }
	XMFLOAT3& GetRight(){ return RightVector; }
	XMFLOAT3& GetUp(){ return UpVector; }

	void NormalizeTransformationVector();
private:
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	XMFLOAT3 ForwardVector;
	XMFLOAT3 RightVector;
	XMFLOAT3 UpVector;
};