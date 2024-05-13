#include "OperationHandleBase.h"
#include "../Engine/Component/Mesh/CustomMeshComponent.h"

AOperationHandleBase::AOperationHandleBase()
	: XAxisComponent({})
	, YAxisComponent({})
	, ZAxisComponent({})
{
}

void AOperationHandleBase::SetBaseColor()
{
	SetBaseColor(XAxisComponent, XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f));
	SetBaseColor(YAxisComponent, XMFLOAT4(0.0f, 1.0f, 0.f, 1.0f));
	SetBaseColor(ZAxisComponent, XMFLOAT4(0.0f, 0.0f, 1.f, 1.0f));
}

void AOperationHandleBase::SetBaseColor(std::shared_ptr<CCustomMeshComponent> InCustomComponent, XMFLOAT4 const& InColor)
{
	if (InCustomComponent.get())
	{
		if (shared_ptr<CMaterial> Material = InCustomComponent->GetMaterials()[0])
		{
			Material->SetBaseColor(InColor);
		}
	}
}

void AOperationHandleBase::SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterialX, shared_ptr<CMaterial> InMaterialY, shared_ptr<CMaterial> InMaterialZ)
{
	XAxisComponent->SetSubMaterials(index, InMaterialX);
	YAxisComponent->SetSubMaterials(index, InMaterialY);
	ZAxisComponent->SetSubMaterials(index, InMaterialZ);
}
