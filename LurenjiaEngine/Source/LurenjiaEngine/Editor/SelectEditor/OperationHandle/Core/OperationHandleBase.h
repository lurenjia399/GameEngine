#pragma once
#include "../Engine/Actor/Core/Actor.h"
#include "../Engine/Interface/DirectXDeviceInterface.h"

class CCustomMeshComponent;
enum class EMeshComponentRenderLayerType;
class CMaterial;
class AOperationHandleBase : public AActor, public IDirectXDeviceInterface
{
	typedef AActor Super;
public:
	AOperationHandleBase();

	virtual void SetMeshComponentLayerType(EMeshComponentRenderLayerType InType){}
	virtual void SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterial){}
	virtual void SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterialX, shared_ptr<CMaterial> InMaterialY, shared_ptr<CMaterial> InMaterialZ);
	void SetBaseColor();
	void SetBaseColor(std::shared_ptr<CCustomMeshComponent> InCustomComponent, XMFLOAT4 const& InColor);
	
protected:
	std::shared_ptr<CCustomMeshComponent> XAxisComponent;
	std::shared_ptr<CCustomMeshComponent> YAxisComponent;
	std::shared_ptr<CCustomMeshComponent> ZAxisComponent;
};