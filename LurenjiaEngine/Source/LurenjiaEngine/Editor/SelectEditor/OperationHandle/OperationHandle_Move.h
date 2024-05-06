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
	void SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterial) override;
};