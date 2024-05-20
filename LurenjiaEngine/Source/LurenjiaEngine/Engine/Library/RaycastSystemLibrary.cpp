#include "RaycastSystemLibrary.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/Camera.h"
#include "../Collision/CollisionSceneQuery.h"

struct FRaycastSystemFunc
{
	static bool GetRaycastByScreenParam(shared_ptr<CWorld> InWorld, XMVECTOR& OutViewOriginPoint,
		XMVECTOR& OutViewDirection, 
		XMMATRIX& OutWorld2ViewMatrixInverse, int ScreenX, int ScreenY)
	{
		if (shared_ptr<ACamera> camera = InWorld->GetCamera())
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
			view.y = (-2.0f * ScreenY / height + 1.0f) * z / ProjectY; // ע��������ţ���Ϊdx���ӿ�ԭ�������Ͻǣ�opengl�������½�

			OutViewOriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.0f); // ������ռ��µ�ԭ�㣬Ҳ������������ڵ�λ��
			XMVECTOR ViewDirection = XMVectorSet(view.x, view.y, 1.0f, 0.f);// ���߷���
			OutViewDirection = XMVector3Normalize(ViewDirection);

			XMMATRIX World2ViewMatrix = XMLoadFloat4x4(&camera->ViewMatrix);
			World2ViewMatrix = XMMatrixTranspose(World2ViewMatrix);//�м���Ҫת�ã��������еľ�����Ҫת�������е�
			XMVECTOR World2ViewMatrixDeterminant = DirectX::XMMatrixDeterminant(World2ViewMatrix);
			OutWorld2ViewMatrixInverse = DirectX::XMMatrixInverse(&World2ViewMatrixDeterminant, World2ViewMatrix); // ���������������

			return true;

		}
		return false;
	}
};

bool FRaycastSystemLibrary::HitResultByScreen(shared_ptr<CWorld> InWorld, int ScreenX, int ScreenY, FHitResult& OutHitResult)
{
	if (shared_ptr<ACamera> camera = InWorld->GetCamera())
	{
		XMVECTOR ViewOriginPoint = {};
		XMVECTOR ViewDirection = {};
		XMMATRIX World2ViewMatrixInverse = {};
		if (FRaycastSystemFunc::GetRaycastByScreenParam(InWorld, ViewOriginPoint, ViewDirection, World2ViewMatrixInverse, ScreenX, ScreenY))
		{
			return FCollisionSceneQuery::RaycastSingle(InWorld, ViewOriginPoint, ViewDirection, World2ViewMatrixInverse, OutHitResult);
		}
		
	}


    return false;
}

bool FRaycastSystemLibrary::HitSpecificObjectsResultByScreen(shared_ptr<CWorld> InWorld, AActor* InSpecificObject, int ScreenX, int ScreenY, FHitResult& OutHitResult)
{
	if (shared_ptr<ACamera> camera = InWorld->GetCamera())
	{
		XMVECTOR ViewOriginPoint = {};
		XMVECTOR ViewDirection = {};
		XMMATRIX World2ViewMatrixInverse = {};
		if (FRaycastSystemFunc::GetRaycastByScreenParam(InWorld, ViewOriginPoint, ViewDirection, World2ViewMatrixInverse, ScreenX, ScreenY))
		{
			return FCollisionSceneQuery::RaycastSingle(InWorld, InSpecificObject, ViewOriginPoint, ViewDirection, World2ViewMatrixInverse, OutHitResult);
		}

	}
	return false;
}
