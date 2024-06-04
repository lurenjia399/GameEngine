#include "OperationHandleBase.h"
#include "../Engine/Component/Mesh/CustomMeshComponent.h"
#include "../Engine/Component/InputComponent.h"
#include "../Engine/Library/RaycastSystemLibrary.h"
#include "../Engine/Collision/CollisionSceneQuery.h"
#include "../Engine/Core/Camera.h"


std::weak_ptr<AActor> AOperationHandleBase::SelectedActor;
std::weak_ptr<CCustomMeshComponent> AOperationHandleBase::SelectedAxisComponent;
bool AOperationHandleBase::bOperationHandleSelect = false;

AOperationHandleBase::AOperationHandleBase()
	: XAxisComponent({})
	, YAxisComponent({})
	, ZAxisComponent({})
	, InputComponent({})
	, SelectDir(XMFLOAT3(0, 0, 0))
	, MousePos2SelectPosOffset(XMFLOAT3(0,0,0))
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

	GetWorld()->GetCamera()->SelectedActorDelegate.AddFunction(this, &AOperationHandleBase::OnSelectedActor);
}

void AOperationHandleBase::OnMouseMove(int X, int Y, string buttonType)
{
	FHitResult HitResult = {};
	bool bHit = FRaycastSystemLibrary::HitSpecificObjectsResultByScreen(GetWorld(), this, X, Y, HitResult);

	if (bHit)
	{
		ResetColor();
		if (!HitResult.Component_.expired())
		{
			std::shared_ptr<CCustomMeshComponent> component = static_pointer_cast<CCustomMeshComponent>(HitResult.Component_.lock());
			SetBaseColor(component, XMFLOAT4(1.0f, 1.0f, 0.f, 1.0f)); 

			AOperationHandleBase::SelectedAxisComponent = component;
		}
	}
	else
	{
		if (!AOperationHandleBase::bOperationHandleSelect)
		{
			ResetColor();
			AOperationHandleBase::SelectedAxisComponent.reset();
		}
	}
	
}

void AOperationHandleBase::OnLeftMouseButtonDown(int X, int Y)
{
	AOperationHandleBase::bOperationHandleSelect = true;

	if (!AOperationHandleBase::SelectedAxisComponent.expired())
	{
		if (AOperationHandleBase::SelectedAxisComponent.lock() == XAxisComponent)
		{
			YAxisComponent->SetVisible(false);
			ZAxisComponent->SetVisible(false);
		}
		else if (AOperationHandleBase::SelectedAxisComponent.lock() == YAxisComponent)
		{
			XAxisComponent->SetVisible(false);
			ZAxisComponent->SetVisible(false);
		}
		else if (AOperationHandleBase::SelectedAxisComponent.lock() == ZAxisComponent)
		{
			XAxisComponent->SetVisible(false);
			YAxisComponent->SetVisible(false);
		}
	}
	
}

void AOperationHandleBase::OnLeftMouseButtonUp(int X, int Y)
{
	AOperationHandleBase::bOperationHandleSelect = false;
	if (!AOperationHandleBase::SelectedAxisComponent.expired())
	{
		ResetColor();
		AOperationHandleBase::SelectedAxisComponent.reset();

		SetVisible(true);
	}
}

void AOperationHandleBase::OnSelectedActor(bool bSelected)
{
	SetVisible(bSelected);

	if (!AOperationHandleBase::SelectedActor.expired())
	{
		const XMFLOAT3 SelectActorPosition = AOperationHandleBase::SelectedActor.lock()->GetPosition();
		SetPosition(SelectActorPosition);
	}
}

void AOperationHandleBase::SetSubMaterials(const int& index, shared_ptr<CMaterial> InMaterialX, shared_ptr<CMaterial> InMaterialY, shared_ptr<CMaterial> InMaterialZ)
{
	XAxisComponent->SetSubMaterials(index, InMaterialX);
	YAxisComponent->SetSubMaterials(index, InMaterialY);
	ZAxisComponent->SetSubMaterials(index, InMaterialZ);
}
