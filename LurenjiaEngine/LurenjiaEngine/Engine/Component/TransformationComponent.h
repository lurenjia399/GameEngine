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

	XMFLOAT3& GetPosition(){ return Position; }
	XMFLOAT3& GetForward(){ return ForwardVector; }
	XMFLOAT3& GetRight(){ return RightVector; }
	XMFLOAT3& GetUp(){ return UpVector; }

	void NormalizeTransformationVector();
	void UpdateCameraInfo();
private:
	XMFLOAT3 Position;

	XMFLOAT3 ForwardVector;
	XMFLOAT3 RightVector;
	XMFLOAT3 UpVector;
};