#include "RaycastSystemLibrary.h"
#include "../Config/EngineRenderConfig.h"
#include "../Core/Camera.h"

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

		XMVECTOR OriginPoint = XMVectorSet(0.f, 0.f, 0.f, 1.0f); // ������ռ��µ�Զ�㣬Ҳ������������ڵ�λ��
		XMVECTOR Direction = XMVectorSet(view.x, view.y, 1.f, 0.f);// ���߷���

	}
	
    //InWorld->LineTraceSingleByChannel(OutHitResult);

    return false;
}
