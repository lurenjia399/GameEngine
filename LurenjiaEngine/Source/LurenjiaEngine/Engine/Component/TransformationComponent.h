#pragma once
#include "Core/Component.h"

class CTransformationComponent : public CCoreMinimalObject
{
public:
	CTransformationComponent();

	virtual void SetPosition(const XMFLOAT3& InPosition);
	virtual void SetRotation(const fvector_3d& InRotation);
	virtual void SetScale(const XMFLOAT3& InScale);

	virtual void SetForward(const XMFLOAT3& InForward);
	virtual void SetRight(const XMFLOAT3& InRight);
	virtual void SetUp(const XMFLOAT3& InUp);

	virtual void SetPositionOffset(const XMFLOAT3& InPosition);

	XMFLOAT3& GetPosition();
	fvector_3d GetRotation(){ return fvector_3d(Rotation.x, Rotation.y, Rotation.z); }
	XMFLOAT3& GetScale() { return Scale; }
	XMFLOAT3& GetForward(){ return ForwardVector; }
	XMFLOAT3& GetRight(){ return RightVector; }
	XMFLOAT3& GetUp(){ return UpVector; }

	void NormalizeTransformationVector();

	std::weak_ptr<CTransformationComponent> GetAttachParent();
	virtual void AttachToComponent(std::weak_ptr<CTransformationComponent> Parent);
	virtual void SetAttachParent(std::weak_ptr<CTransformationComponent> Parent);
	virtual void UpdateComponentPositionToWorldWithParent(std::weak_ptr<CTransformationComponent> Parent, const XMFLOAT3& InPosition);
	virtual void UpdateComponentRotationToWorldWithParent(std::weak_ptr<CTransformationComponent> Parent, const fvector_3d& InRotation);
	virtual void UpdateComponentScaleToWorldWithParent(std::weak_ptr<CTransformationComponent> Parent, const XMFLOAT3& InScale);
	virtual void UpdateChildPosition(const XMFLOAT3& InPosition);
	virtual void UpdateChildRotation(const fvector_3d& InRotation);
	virtual void UpdateChildScale(const XMFLOAT3& InScale);
	
private:
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	XMFLOAT3 ForwardVector;
	XMFLOAT3 RightVector;
	XMFLOAT3 UpVector;

	XMFLOAT3 PositionOffset;//子compoent相对于父component的位置偏移

	std::weak_ptr<CTransformationComponent> AttachParent;
	std::vector<std::weak_ptr<CTransformationComponent>> AttachChildren;
};