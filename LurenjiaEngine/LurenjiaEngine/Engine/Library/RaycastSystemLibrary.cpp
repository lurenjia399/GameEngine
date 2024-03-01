#include "RaycastSystemLibrary.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/Camera.h"
#include "../Collision/CollisionSceneQuery.h"

bool FRaycastSystemLibrary::HitResultByScreen(CWorld* InWorld, int ScreenX, int ScreenY, FHitResult& OutHitResult)
{
	if (ACamera* camera = InWorld->GetCamera())
	{
		// ���������Ҫ���ľ���:
		// ���ӿڿռ� �任�� ������ռ�
		// �ӿڿռ� -> ndc�ռ� -> ͸�ӳ˷� -> ͶӰ������� = ������ռ�

		float z = 1.0f;//͸�ӳ˷���z�ᣬ��Ϊ������Ҫ֪���������Բ���Ҫ��ȷ���������ռ��¶�Ӧ�������ֵ��

		int width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
		int height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

		float ProjectX = camera->ProjectMatrix._11;
		float ProjectY = camera->ProjectMatrix._22;

		XMFLOAT2 view = XMFLOAT2(0.f, 0.f);// ���������zΪ1������£�������ռ��µ�λ��
		view.x = (2.0f * ScreenX / width - 1.0f) * z / ProjectX;
		view.y = (2.0f * ScreenY / height - 1.0f) * z / ProjectY;

		XMVECTOR ViewOriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.0f); // ������ռ��µ�Զ�㣬Ҳ������������ڵ�λ��
		XMVECTOR ViewDirection = XMVectorSet(view.x, view.y, 1.f, 0.f);// ���߷���


		XMMATRIX World2ViewMatrix = XMLoadFloat4x4(&camera->ViewMatrix);
		XMVECTOR World2ViewMatrixDeterminant = DirectX::XMMatrixDeterminant(World2ViewMatrix);
		XMMATRIX World2ViewMatrixInverse = DirectX::XMMatrixInverse(&World2ViewMatrixDeterminant, World2ViewMatrix); // ���������������

		FHitResult HitResult = {};
		FCollisionSceneQuery::RaycastSingle(InWorld, ViewOriginPoint, ViewDirection, World2ViewMatrixInverse, HitResult);
	}
	
    //InWorld->LineTraceSingleByChannel(OutHitResult);

    return false;
}
