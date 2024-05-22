#include "OperationHandle_Move.h"
#include "../Engine/Actor/Mesh/Core/MeshManage.h"
#include "../Engine/Component/Mesh/CustomMeshComponent.h"
#include "../Engine/Core/Construction/MacroConstruction.h"
#include "../Engine/Library/RaycastSystemLibrary.h"
#include "../Engine/Collision/CollisionSceneQuery.h"

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

	SetMeshComponentLayerType(EMeshComponentRenderLayerType::RENDERLAYER_OPERATIONHANDLE);
	
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

void AOperationHandle_Move::SetMeshComponentLayerType(EMeshComponentRenderLayerType InType)
{
	// 设置渲染层级
	XAxisComponent->SetMeshComponentLayerType(InType);
	YAxisComponent->SetMeshComponentLayerType(InType);
	ZAxisComponent->SetMeshComponentLayerType(InType);
}

void AOperationHandle_Move::OnMouseMove(int X, int Y, string buttonType)
{
	Super::OnMouseMove(X, Y, buttonType);
	return; // 这里先关掉，测试别的功能

	if (std::shared_ptr<AActor> SelectedActor_SharedPtr = Super::SelectedActor.lock())
	{
		ESelectAxisType SelectAxisType = GetSelectAxis();
		if (SelectAxisType == ESelectAxisType::SELECTAXISTYPE_NONE)
		{
			Engine_Log_Error("ESelectAxisType::SELECTAXISTYPE_NONE");
			return;
		}

		XMVECTOR ViewOriginPoint = {};
		XMVECTOR ViewDirection = {};
		XMMATRIX World2ViewMatrixInverse = {};
		if (FRaycastSystemLibrary::GetRaycastByScreenParam(GetWorld(), ViewOriginPoint, ViewDirection, World2ViewMatrixInverse, X, Y))
		{
			// 射线的方向和原点
			XMVECTOR WorldOriginPoint = DirectX::XMVector3TransformCoord(ViewOriginPoint, World2ViewMatrixInverse);
			XMVECTOR WorldDirection = DirectX::XMVector3TransformNormal(ViewDirection, World2ViewMatrixInverse);
			WorldDirection = DirectX::XMVector3Normalize(WorldDirection);
			// 物体的方向和原点
			XMFLOAT3 SelectActorPos = SelectedActor_SharedPtr->GetPosition();
			XMVECTOR SelectActorPostion = XMLoadFloat3(&SelectActorPos);
			XMFLOAT3 SelectActorDir = XMFLOAT3(0, 0, 0);
			switch (SelectAxisType)
			{
			case ESelectAxisType::SELECTAXISTYPE_X:
				SelectActorDir = SelectedActor_SharedPtr->GetForward();
				break;
			case ESelectAxisType::SELECTAXISTYPE_Y:
				SelectActorDir = SelectedActor_SharedPtr->GetRight();
				break;
			case ESelectAxisType::SELECTAXISTYPE_Z:
				SelectActorDir = SelectedActor_SharedPtr->GetUp();
				break;
			default:
				Engine_Log_Error("switch (GetSelectAxis()) error");
				break;
			}
			XMVECTOR SelectActorDirection = XMLoadFloat3(&SelectActorDir);

			XMVECTOR v1_Corss_v2 = DirectX::XMVector3Cross(WorldDirection, SelectActorDirection);
			float v1_Corss_v2_len = DirectX::XMVector3Length(v1_Corss_v2).m128_f32[0];
			float t1 = DirectX::XMVector3Dot(
							DirectX::XMVector3Cross(DirectX::XMVectorSubtract(SelectActorPostion ,WorldOriginPoint), 
										SelectActorDirection),
							v1_Corss_v2).m128_f32[0]
						/ (v1_Corss_v2_len * v1_Corss_v2_len);
			XMVECTOR ResultPos = DirectX::XMVectorAdd(WorldOriginPoint, DirectX::XMVectorScale(WorldDirection, t1));
			XMFLOAT3 ResultPostion = XMFLOAT3();
			XMStoreFloat3(&ResultPostion, ResultPos);
			SelectedActor_SharedPtr->SetPosition(ResultPostion);
		}
		else {
			Engine_Log_Error("FRaycastSystemLibrary::GetRaycastByScreenParam is error");
			return;
		}
	}
}

void AOperationHandle_Move::OnLeftMouseButtonDown(int X, int Y)
{
	Super::OnLeftMouseButtonDown(X, Y);
}

void AOperationHandle_Move::OnLeftMouseButtonUp(int X, int Y)
{
	Super::OnLeftMouseButtonUp(X, Y);
}
