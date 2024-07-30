#pragma once
#include "Component/Component.h"
#include "../../Engine/LurenjiaEngine.h"

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

	CTransformationComponent* GetAttachParent();
	virtual void AttachToComponent(CTransformationComponent* Parent);
	virtual void SetAttachParent(CTransformationComponent* Parent);
	virtual void UpdateComponentPositionToWorldWithParent(CTransformationComponent* Parent, const XMFLOAT3& InPosition);
	virtual void UpdateComponentRotationToWorldWithParent(CTransformationComponent* Parent, const fvector_3d& InRotation);
	virtual void UpdateComponentScaleToWorldWithParent(CTransformationComponent* Parent, const XMFLOAT3& InScale);
	virtual void UpdateChildPosition(const XMFLOAT3& InPosition);
	virtual void UpdateChildRotation(const fvector_3d& InRotation);
	virtual void UpdateChildScale(const XMFLOAT3& InScale);
	
	void SetVisible(bool bNewVisible);
	bool GetIsVisible() const;
private:
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	XMFLOAT3 ForwardVector;
	XMFLOAT3 RightVector;
	XMFLOAT3 UpVector;

	XMFLOAT3 PositionOffset;//子compoent相对于父component的位置偏移

	CTransformationComponent* AttachParent;
	std::vector<CTransformationComponent*> AttachChildren;

	bool bIsVisible;//是否可见，也就是是否渲染
};