#pragma once
#include "Core/OperationHandleBase.h"

class AOperationHandle_Move : public AOperationHandleBase
{
	typedef AOperationHandleBase Super;
public:
	void SetPosition(const XMFLOAT3& InPosition) override;
	AOperationHandle_Move();
	void CreateComponent();

	void SetMeshComponentLayerType(EMeshComponentRenderLayerType InType) override;

	virtual void OnMouseMove(int X, int Y, string buttonType) override;
	virtual void OnLeftMouseButtonDown(int X, int Y) override;
	virtual void OnLeftMouseButtonUp(int X, int Y) override;
};