#include "OperationHandleBase.h"
#include "../Engine/Component/Mesh/CustomMeshComponent.h"
#include "../Engine/Component/InputComponent.h"
#include "../Engine/Library/RaycastSystemLibrary.h"
#include "../Engine/Collision/CollisionSceneQuery.h"


std::weak_ptr<AActor> AOperationHandleBase::SelectedActor;
std::weak_ptr<CCustomMeshComponent> AOperationHandleBase::SelectedAxisComponent;

AOperationHandleBase::AOperationHandleBase()
	: XAxisComponent({})
	, YAxisComponent({})
	, ZAxisComponent({})
	, InputComponent({})
{
	InputComponent = LurenjiaEngine::CreateObject<CInputComponent>(this, "AOperationHandleBaseInputComponent");
}

void AOperationHandleBase::SetBaseColor()
{
	ResetColor();
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

void AOperationHandleBase::ResetColor()
{
	SetBaseColor(XAxisComponent, XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f));
	SetBaseColor(YAxisComponent, XMFLOAT4(0.0f, 1.0f, 0.f, 1.0f));
	SetBaseColor(ZAxisComponent, XMFLOAT4(0.0f, 0.0f, 1.f, 1.0f));
}

ESelectAxisType AOperationHandleBase::GetSelectAxis()
{
	ESelectAxisType result = ESelectAxisType::SELECTAXISTYPE_NONE;
	if (AOperationHandleBase::SelectedAxisComponent.lock() == XAxisComponent)
	{
		result = ESelectAxisType::SELECTAXISTYPE_X;
	}
	else if (AOperationHandleBase::SelectedAxisComponent.lock() == YAxisComponent)
	{
		result = ESelectAxisType::SELECTAXISTYPE_Y;
	}
	else if (AOperationHandleBase::SelectedAxisComponent.lock() == ZAxisComponent)
	{
		result = ESelectAxisType::SELECTAXISTYPE_Z;
	}
	return result;
}

void AOperationHandleBase::BeginInit()
{
	Super::BeginInit();

	InputComponent->OnMouseMoveDelegate.Bind(this, &AOperationHandleBase::OnMouseMove);
	InputComponent->OnLMouseButtonDownDelegate.Bind(this, &AOperationHandleBase::OnLeftMouseButtonDown);
	InputComponent->OnLMouseButtonUpDelegate.Bind(this, &AOperationHandleBase::OnLeftMouseButtonUp);

}

void AOperationHandleBase::OnMouseMove(int X, int Y, string buttonType)
{
	FHitResult HitResult = {};
	bool bHit = FRaycastSystemLibrary::HitSpecificObjectsResultByScreen(GetWorld(), this, X, Y, HitResult);

	ResetColor();

	if (bHit)
	{
		if (!HitResult.Component_.expired())
		{
			std::shared_ptr<CCustomMeshComponent> component = static_pointer_cast<CCustomMeshComponent>(HitResult.Component_.lock());
			SetBaseColor(component, XMFLOAT4(1.0f, 1.0f, 0.f, 1.0f)); 

			AOperationHandleBase::SelectedAxisComponent = component;
		}
	}
	else
	{
		AOperationHandleBase::SelectedAxisComponent.reset();
	}
	
}

void AOperationHandleBase::OnLeftMouseButtonDown(int X, int Y)
{
	//Engine_Log("AOperationHandleBase::OnLeftMouseButtonDown x = [%d], y = [%d]", X, Y)
}

void AOperationHandleBase::OnLeftMouseButtonUp(int X, int Y)
{
	//Engine_Log("AOperationHandleBase::OnLeftMouseButtonUp x = [%d], y = [%d]", X, Y)
}

void AOperationHandleBase::SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterialX, shared_ptr<CMaterial> InMaterialY, shared_ptr<CMaterial> InMaterialZ)
{
	XAxisComponent->SetSubMaterials(index, InMaterialX);
	YAxisComponent->SetSubMaterials(index, InMaterialY);
	ZAxisComponent->SetSubMaterials(index, InMaterialZ);
}
