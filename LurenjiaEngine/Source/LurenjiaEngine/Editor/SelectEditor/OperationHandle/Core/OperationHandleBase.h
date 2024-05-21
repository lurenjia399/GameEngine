#pragma once
#include "../Engine/Actor/Core/Actor.h"
#include "../Engine/Interface/DirectXDeviceInterface.h"

enum class ESelectAxisType
{
	SELECTAXISTYPE_NONE,
	SELECTAXISTYPE_X,
	SELECTAXISTYPE_Y,
	SELECTAXISTYPE_Z,
};

class CCustomMeshComponent;
enum class EMeshComponentRenderLayerType;
class CMaterial;
class CInputComponent;
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
	void ResetColor();
	ESelectAxisType GetSelectAxis();

	virtual void BeginInit() override;
protected:
	virtual void OnMouseMove(int X, int Y, string buttonType);
	virtual void OnLeftMouseButtonDown(int X, int Y);
	virtual void OnLeftMouseButtonUp(int X, int Y);
protected:
	std::shared_ptr<CCustomMeshComponent> XAxisComponent;
	std::shared_ptr<CCustomMeshComponent> YAxisComponent;
	std::shared_ptr<CCustomMeshComponent> ZAxisComponent;

	std::shared_ptr<CInputComponent> InputComponent;
public:
	static std::weak_ptr<AActor> SelectedActor;
	static std::weak_ptr<CCustomMeshComponent> SelectedAxisComponent;
};