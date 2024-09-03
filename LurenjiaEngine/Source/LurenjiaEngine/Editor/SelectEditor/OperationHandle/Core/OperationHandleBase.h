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
	void SetBaseColor(CCustomMeshComponent* InCustomComponent, XMFLOAT4 const& InColor);
	void ResetColor();
	ESelectAxisType GetSelectAxis();

	static bool bIsCaptureInSceneMouse();

	virtual void BeginInit() override;
protected:
	virtual void OnMouseMove(int X, int Y, string buttonType);
	virtual void OnLeftMouseButtonDown(int X, int Y);
	virtual void OnLeftMouseButtonUp(int X, int Y);
	void OnSelectedActor(bool bSelected);
protected:
	CCustomMeshComponent* XAxisComponent;
	CCustomMeshComponent* YAxisComponent;
	CCustomMeshComponent* ZAxisComponent;

	CInputComponent* InputComponent;
	XMFLOAT3 SelectDir;
	XMFLOAT3 MousePos2SelectPosOffset;//鼠标选中位置和选中物体间的偏移
public:
	static AActor* SelectedActor;
	static CCustomMeshComponent* SelectedAxisComponent;
	static bool bOperationHandleSelect;// 是否按住了操作手柄

};