#pragma once
#include "Core/Component.h"

class CTransformationComponent : public CCoreMinimalObject
{
public:
	CTransformationComponent();

	void SetPosition(const XMFLOAT3& InPosition) { Position = InPosition; }
	void SetForward(const XMFLOAT3& InForward) { ForwardVector = InForward; }
	void SetRight(const XMFLOAT3& InRight) { RightVector = InRight; }
	void SetUp(const XMFLOAT3& InUp) { UpVector = InUp; }

	XMFLOAT3 GetPosition()const { return Position; }
	XMFLOAT3 GetForward()const { return ForwardVector; }
	XMFLOAT3 GetRight()const { return RightVector; }
	XMFLOAT3 GetUp()const { return UpVector; }

	void CorrectionVector(fvector_3d& InV3);
	void GetCorrectionPosition(fvector_3d& InV3);
private:
	XMFLOAT3 Position;

	XMFLOAT3 ForwardVector;
	XMFLOAT3 RightVector;
	XMFLOAT3 UpVector;
};