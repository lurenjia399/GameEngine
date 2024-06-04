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

void AOperationHandle_Move::SetVisible(bool InVisible)
{
	if (RootComponent.use_count() > 0)
	{
		RootComponent->SetVisible(InVisible);
	}
	else
	{
		XAxisComponent->SetVisible(InVisible);
		YAxisComponent->SetVisible(InVisible);
		ZAxisComponent->SetVisible(InVisible);
	}
}

void AOperationHandle_Move::SetPickup(bool InVisible)
{
	XAxisComponent->SetPickup(InVisible);
	YAxisComponent->SetPickup(InVisible);
	ZAxisComponent->SetPickup(InVisible);
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

	XAxisComponent->AttachToComponent(RootComponent);
	YAxisComponent->AttachToComponent(RootComponent);
	ZAxisComponent->AttachToComponent(RootComponent);
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

	if (std::shared_ptr<AActor> SelectedActor_SharedPtr = Super::SelectedActor.lock())
	{
		ESelectAxisType SelectAxisType = GetSelectAxis();
		if (SelectAxisType == ESelectAxisType::SELECTAXISTYPE_NONE)
		{
			
			return;
		}
		//Engine_Log_Error("SelectAxisType = %d", SelectAxisType);
		if (!AOperationHandleBase::bOperationHandleSelect)
		{
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
			XMVECTOR SelectActorPostion = DirectX::XMLoadFloat3(&SelectActorPos);
			XMFLOAT3 SelectActorDir = XMFLOAT3(0, 0, 0);
			switch (SelectAxisType)
			{
			case ESelectAxisType::SELECTAXISTYPE_X:
				SelectActorDir = SelectedActor_SharedPtr->GetForward(); // GetForward
				break;
			case ESelectAxisType::SELECTAXISTYPE_Y:
				SelectActorDir = SelectedActor_SharedPtr->GetRight(); // GetRight
				break;
			case ESelectAxisType::SELECTAXISTYPE_Z:
				SelectActorDir = SelectedActor_SharedPtr->GetUp(); // GetUp
				break;
			default:
				Engine_Log_Error("switch (GetSelectAxis()) error");
				break;
			}
			XMVECTOR SelectActorDirectionV = DirectX::XMLoadFloat3(&SelectActorDir);
			SelectActorDirectionV = DirectX::XMVector3Normalize(SelectActorDirectionV);

			XMVECTOR v1_Corss_v2 = DirectX::XMVector3Cross(WorldDirection, SelectActorDirectionV);
			float v1_Corss_v2_len = DirectX::XMVector3Length(v1_Corss_v2).m128_f32[0];
			float v1_Corss_v2_len_pow2 = DirectX::XMVector3LengthSq(v1_Corss_v2).m128_f32[0];
			float t1 = DirectX::XMVector3Dot(
				v1_Corss_v2,
				DirectX::XMVector3Cross(WorldDirection, DirectX::XMVectorSubtract(WorldOriginPoint, SelectActorPostion))
			).m128_f32[0]
				/ v1_Corss_v2_len_pow2;

			XMVECTOR ResultPosV = DirectX::XMVectorAdd(SelectActorPostion, DirectX::XMVectorScale(SelectActorDirectionV, t1));
			XMFLOAT3 ResultPostion = XMFLOAT3();
			//XMVECTOR ResultMove = DirectX::XMVectorMultiply(ResultPosV, SelectActorDirectionV);
			DirectX::XMStoreFloat3(&ResultPostion, ResultPosV);

			// 鼠标位置 - 物体位置 = 移动距离
			XMFLOAT3 SelectPosition = SelectedActor_SharedPtr->GetPosition();
			XMVECTOR SelectPositionV = DirectX::XMLoadFloat3(&SelectPosition);
			XMVECTOR MoveDistance = DirectX::XMVectorSubtract(ResultPosV, SelectPositionV);

			// 移动距离 - 偏移 = 移动步经
			XMVECTOR MousePos2SelectPosOffsetV = DirectX::XMLoadFloat3(&MousePos2SelectPosOffset);
			XMVECTOR MoveStep = DirectX::XMVectorSubtract(MoveDistance, MousePos2SelectPosOffsetV);
			
			// 物体位置 + 移动步经 = 物体最终位置
			XMVECTOR TargetPositionV = DirectX::XMVectorAdd(MoveStep, SelectPositionV);
			XMFLOAT3 TargetPosition = XMFLOAT3();
			DirectX::XMStoreFloat3(&TargetPosition, TargetPositionV);

			// 设置物体和手柄位置
			SelectedActor_SharedPtr->SetPosition(TargetPosition);
			SetPosition(TargetPosition);
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

	

	if (std::shared_ptr<AActor> SelectedActor_SharedPtr = Super::SelectedActor.lock())
	{
		ESelectAxisType SelectAxisType = GetSelectAxis();
		if (SelectAxisType == ESelectAxisType::SELECTAXISTYPE_NONE)
		{
			//Engine_Log_Error("ESelectAxisType::SELECTAXISTYPE_NONE");
			return;
		}

		if (!AOperationHandleBase::bOperationHandleSelect)
		{
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
			XMVECTOR SelectActorPostion = DirectX::XMLoadFloat3(&SelectActorPos);
			XMFLOAT3 SelectActorDir = XMFLOAT3(0, 0, 0);
			switch (SelectAxisType)
			{
			case ESelectAxisType::SELECTAXISTYPE_X:
				SelectActorDir = SelectedActor_SharedPtr->GetForward(); // GetForward
				break;
			case ESelectAxisType::SELECTAXISTYPE_Y:
				SelectActorDir = SelectedActor_SharedPtr->GetRight(); // GetRight
				break;
			case ESelectAxisType::SELECTAXISTYPE_Z:
				SelectActorDir = SelectedActor_SharedPtr->GetUp(); // GetUp
				break;
			default:
				Engine_Log_Error("switch (GetSelectAxis()) error");
				break;
			}
			XMVECTOR SelectActorDirection = DirectX::XMLoadFloat3(&SelectActorDir);
			SelectActorDirection = DirectX::XMVector3Normalize(SelectActorDirection);

			XMVECTOR v1_Corss_v2 = DirectX::XMVector3Cross(WorldDirection, SelectActorDirection);
			float v1_Corss_v2_len = DirectX::XMVector3Length(v1_Corss_v2).m128_f32[0];
			float v1_Corss_v2_len_pow2 = DirectX::XMVector3LengthSq(v1_Corss_v2).m128_f32[0];
			float t1 = DirectX::XMVector3Dot(
				v1_Corss_v2,
				DirectX::XMVector3Cross(WorldDirection, DirectX::XMVectorSubtract(WorldOriginPoint, SelectActorPostion))
			).m128_f32[0]
				/ v1_Corss_v2_len_pow2;

			XMVECTOR ResultPosV = DirectX::XMVectorAdd(SelectActorPostion, DirectX::XMVectorScale(SelectActorDirection, t1));
			XMFLOAT3 ResultPostion = XMFLOAT3();
			DirectX::XMStoreFloat3(&ResultPostion, ResultPosV);
			
			XMFLOAT3 SelectPosition = SelectedActor_SharedPtr->GetPosition();
			XMVECTOR SelectPositionV = DirectX::XMLoadFloat3(&SelectPosition);
			XMVECTOR MousePos2SelectPosOffsetV = DirectX::XMVectorSubtract(ResultPosV, SelectPositionV);
			DirectX::XMStoreFloat3(&MousePos2SelectPosOffset, MousePos2SelectPosOffsetV);

			//SelectedActor_SharedPtr->SetPosition(ResultPostion);
			//SetPosition(ResultPostion);
		}
		else {
			Engine_Log_Error("FRaycastSystemLibrary::GetRaycastByScreenParam is error");
			return;
		}
	}
	
	
}

void AOperationHandle_Move::OnLeftMouseButtonUp(int X, int Y)
{
	Super::OnLeftMouseButtonUp(X, Y);

	
}
