#include "OperationHandle_Move.h"
#include "../Engine/Actor/Mesh/Core/MeshManage.h"
#include "../Engine/Component/Mesh/CustomMeshComponent.h"
#include "../Engine/Core/Construction/MacroConstruction.h"

void AOperationHandle_Move::SetPosition(const XMFLOAT3& InPosition)
{
	Super::SetPosition(InPosition);
	
	assert(XAxisComponent);
	assert(YAxisComponent);
	assert(ZAxisComponent);

	XAxisComponent->SetPosition(InPosition);
	YAxisComponent->SetPosition(InPosition);
	ZAxisComponent->SetPosition(InPosition);
}

AOperationHandle_Move::AOperationHandle_Move()
{
	XAxisComponent = LurenjiaEngine::CreateObject<CCustomMeshComponent>(this, "XAxisComponent");
	YAxisComponent = LurenjiaEngine::CreateObject<CCustomMeshComponent>(this, "YAxisComponent");
	ZAxisComponent = LurenjiaEngine::CreateObject<CCustomMeshComponent>(this, "ZAxisComponent");
}

void AOperationHandle_Move::CreateComponent()
{
	string ContentPath = FEnginePathHelper::GetEngineContentFBXPath();
	string CustomPath = "/MoveArrow.fbx";
	
	InjectComponentRenderDataByComponent(CCustomMeshComponent, XAxisComponent, ContentPath + CustomPath);
	InjectComponentRenderDataByComponent(CCustomMeshComponent, YAxisComponent, ContentPath + CustomPath);
	InjectComponentRenderDataByComponent(CCustomMeshComponent, ZAxisComponent, ContentPath + CustomPath);

	XAxisComponent->SetRotation(fvector_3d(0, 90, 0));
	YAxisComponent->SetRotation(fvector_3d(-90, 0, 0));
}
